////////////////////////////////////////////////////////////////////////////
//	Created		: 04.03.2009
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_world.h"
#include "game.h"
//#include "camera_director.h"
#include "cell_manager.h"
#include "bullet_manager.h"
#include "event_manager.h"
#include "object_scene.h"
//#include "hud.h"
#include "test_anim_object.h"
#include "actor.h"
#include "actor_input_controller.h"
#include "free_fly_camera.h"
#include "stats.h"

#include <xray/sound/world.h>
#include <xray/sound/world_user.h>
#include <xray/console_command.h>

#include <xray/network/packet_reader.h>

#include <xray/render/facade/scene_renderer.h>

//static void server_on_packet_received	( xray::network::server& server, xray::network::client_session& client, xray::network::packet_reader& packet )
//{
//	string256 message;
//	packet.r_string						( message );
//
//	xray::network::packet* const answer_packet = NEW (xray::network::packet) ( *stalker2::g_allocator );
//	answer_packet->append				( message );
//	server.send							( client, answer_packet );
//}
//
static void client_on_packet_received	( xray::network::client& client, xray::network::packet_reader& packet )
{
	string256 message;
	packet.r_string						( message );

	xray::network::packet* const answer_packet = NEW (xray::network::packet) ( *stalker2::g_allocator );
	answer_packet->append				( "the second message" );
	client.send							( answer_packet );
}

static void client_on_connected			( xray::network::client& client )
{
	xray::network::packet* const packet = NEW (xray::network::packet) ( *stalker2::g_allocator );
	packet->append						( "Hello, world!" );
	client.send							( packet );
}

static xray::command_line::key	cl_cam("cam", "", "", "free camera");
static xray::command_line::key	cl_actor("actor", "", "", "force actor");

namespace stalker2 {


extern pcstr  editor_project_ext;
extern pcstr  resources_converted_path;
extern pcstr  resources_path;


game_world::game_world( game& game )
:super					( game ),
m_collision_tree		( NULL ),
m_game_time_ms			( 0 ),
m_game_time_sec			( 0 ),
m_last_frame_time_ms	( 0 ),
m_last_frame_time_sec	( 0 ),
//m_physics_world			( NULL ),
//m_test_anim_object	( NULL ),
m_bullet_manager		( 0 ),
m_local_actor			( 0 ),
//m_server				( game.get_network_world() ),
m_client				( game.get_network_world() ),
m_scenes_check			( 0 ),
m_actor_spawned			( false ),
m_key_camera			( cl_cam ),
m_key_actor				( cl_actor )
{
	init_physics						( );

	m_cell_manager		= NEW(cell_manager)( game, *this),
#ifndef MASTER_GOLD
	m_dbg_name							= "Game World";
#endif //#ifndef MASTER_GOLD
	m_bullet_manager					= NEW(bullet_manager)( *this );
	m_camera_director					= NEW(camera_director)( *this );
	m_camera_director->set_no_delete	( );
	m_actor_input_controller			= NEW(actor_input_controller)( *this );
	m_free_fly_camera					= NEW(free_fly_camera)(*this, m_camera_director);
//	m_test_anim_object					= NEW(test_anim_object)(*this);
	query_resources			( );

	u16 const port					= 2510;

//	m_server.set_on_packet_received	( boost::bind( &server_on_packet_received, boost::ref(m_server), _1, _2) );
//	m_server.start_accepting		( port, 16 );

	m_client.set_on_packet_received	( boost::bind( &client_on_packet_received, boost::ref(m_client), _1) );
	m_client.set_on_connected		( boost::bind( &client_on_connected, boost::ref(m_client)) );
	m_client.connect				( "localhost", port );
}

game_world::~game_world( )
{
	DELETE				( m_free_fly_camera );
//	DELETE				( m_hud );
	DELETE				( m_local_actor );
	DELETE				( m_actor_input_controller );

	camera_director* cd = m_camera_director.c_ptr(); 
	m_camera_director	= NULL;
	DELETE				( cd );
//	DELETE				( m_test_anim_object );
}

void game_world::time_update( )
{
	u64 elapsed_ticks					= m_timer.get_elapsed_ticks( );
	float new_game_time_sec				= ( float( double(elapsed_ticks) / timing::g_qpc_per_second ) );
	u32 new_game_time_ms				= ( u32( (elapsed_ticks*u64( 1000 ) / timing::g_qpc_per_second) & u32(-1) ) );

	m_last_frame_time_sec				= new_game_time_sec - m_game_time_sec;
	m_game_time_sec						= new_game_time_sec;
	m_last_frame_time_ms				= new_game_time_ms - m_game_time_ms;
	m_game_time_ms						= new_game_time_ms;
}

void game_world::tick()
{
	super::tick();
	time_update();

	m_camera_director->tick();

	if (m_local_actor)
	{
		m_local_actor->tick();
		if (m_local_actor->character_camera_transform().valid() || m_local_actor->character_head_transform().valid()) {
			m_actor_input_controller->inverted_view_matrix() = m_local_actor->character_select_transform();
			//m_actor_input_controller->update_camera_matrix(m_actor_input_controller->m_frame_events, m_actor_input_controller->inverted_view_matrix());
		}
	}

	//if (m_test_anim_object)
	//	m_test_anim_object->tick();

	scenes_list::iterator it = m_active_scenes.begin();
	scenes_list::iterator it_e = m_active_scenes.end();
	for (; it != it_e; ++it)
		(*it)->tick();

	//	if (!get_game().get_active_scene())
	//		return;

	m_camera_director->apply();
	//	if(g_physics_enabled)
	//		m_physics_world->tick			( );

		m_cell_manager->set_inv_view_matrix(m_camera_director->get_inverted_view_matrix());
		m_cell_manager->tick();
		m_bullet_manager->tick(m_game_time_sec);

	get_game().get_sound_world().get_logic_world_user().set_listener_properties_interlocked(
		get_sound_scene(),
		m_inverted_view_matrix.c.xyz(),
		m_inverted_view_matrix.k.xyz(),
		m_inverted_view_matrix.j.xyz());

	get_game().get_stats().set_camera_stats	( m_inverted_view_matrix.c.xyz(), m_inverted_view_matrix.k.xyz() );

}

u32 game_world::game_time_ms	( )
{
	return m_game_time_ms;
}
float game_world::game_time_sec	( )
{
	return m_game_time_sec;
}

u32 game_world::last_frame_time_ms ( )
{
	return m_last_frame_time_ms;
}

float game_world::last_frame_time_sec ( )
{
	return m_last_frame_time_sec;
}

void game_world::unload( )
{
//	DELETE(volumetric_test);

	scenes_list::iterator it = m_active_scenes.begin();
	scenes_list::iterator it_e = m_active_scenes.end();
	for( ;it!=it_e; ++it)
	{
		(*it)->stop			( true );
	}

	if (m_local_actor->get_particle_timer())
	{
		m_local_actor->clear_weapon_fx();
	}

	switch_to_free_fly_camera	( );
	DELETE						( m_local_actor );

	m_active_scenes.clear				( );
	m_camera_director->switch_to_camera	( NULL, "null" );
	m_cell_manager->unload				( );
	ASSERT								( empty() );
	xray::collision::delete_space_partitioning_tree( m_collision_tree );
	m_collision_tree					= NULL;
	m_game_project						= NULL;
}

void game_world::switch_to_hud_camera( )
{
//	m_camera_director->switch_to_camera(m_hud, "hud game camera");

	if (!m_local_actor) {
		spawn_actor();
		return;
	}

		m_camera_director->switch_to_camera	( m_actor_input_controller, "actor camera" );
}

void game_world::switch_to_free_fly_camera( )
{
	m_camera_director->switch_to_camera	( m_free_fly_camera, "free fly camera" );
}

bool game_world::empty( )
{
	return		m_cell_manager->empty();
}

void game_world::load( pcstr project_resource_name, pcstr project_resource_path )
{
	if ( !empty() )
		unload();

	m_collision_tree = &( *( collision::new_space_partitioning_tree( g_allocator, 1.f, 1024 ) ) );

	if ( project_resource_path )
		m_project_resource_path				= project_resource_path;
	else
		m_project_resource_path				= project_resource_name;


	LOG_INFO( "game_world::load : %s", project_resource_name );
	resources::query_resource(
		project_resource_name,
		resources::game_project_class,
		boost::bind( &game_world::on_project_loaded, this, _1 ),
		g_allocator
	);
}

void game_world::spawn_actor() 
{
	m_local_actor = NEW(actor)(*this);
	m_actor_spawned = true;
	m_local_actor->query_resources();
}

void game_world::on_project_loaded( resources::queries_result& data )
{
	R_ASSERT						( data.is_successful() );

	m_game_project					= static_cast_resource_ptr<game_project_ptr>(data[0].get_unmanaged_resource());
	m_cell_manager->load			( m_game_project );

	math::float3 camera_position	= (*m_game_project->m_config)["camera"]["position"];
	math::float3 camera_direction	= (*m_game_project->m_config)["camera"]["direction"];
	m_camera_director->set_position_direction( camera_position, camera_direction );
	m_free_fly_camera->set_position_direction( camera_position, camera_direction );

	//switch_to_hud_camera();

	if(!m_key_camera) {
		switch_to_free_fly_camera();

		m_local_actor = NEW(actor)(*this);
		m_actor_spawned	= true;
		}

		//m_hud = NEW(hud)( *this);
}

void game_world::on_activate( )
{
	super::on_activate			( );
	m_camera_director->on_focus	( true );
	
	if(get_sound_scene())
		get_game().get_sound_world().get_logic_world_user().set_active_sound_scene( get_sound_scene(), 0, 0 );

if (m_key_camera)
	switch_to_free_fly_camera();
}

void game_world::on_deactivate( )
{
	super::on_deactivate		( );
	m_camera_director->on_focus	( false );
}

game_project_ptr const game_world::get_project( ) const
{
	return m_cell_manager->get_project( );
}

game_object_ptr_ game_world::get_object_by_name( pcstr object_id ) const
{
	return m_cell_manager->get_object_by_name( object_id );
}

void game_world::query_object_by_name	( pcstr object_id, object_loaded_callback_type const& callback ) const
{
	return m_cell_manager->query_object_by_name( object_id, callback );
}

void game_world::start_game( )
{
	m_timer.start			( );
	time_update				( );

//.	m_test_anim_object	= NEW(test_anim_object)(*this);

	camera_director_ptr object_ptr			= get_camera_director();
	m_cell_manager->m_named_registry["camera_director"]		= object_ptr.c_ptr();
	
	configs::binary_config_value scenes_to_start = (*m_game_project->m_config)["start"]["scenes_to_start"];
	for ( u32 i = 0; i < scenes_to_start.size(); i++ )
	{
		pcstr start_scene		= (pcstr)scenes_to_start[i];
		game_object_ptr_ s		= get_object_by_name(start_scene);
		object_scene_ptr scene	= static_cast_resource_ptr<object_scene_ptr>(s);
		if (scene.m_object) {
			scene->start();
		}
		else {
			LOG_ERROR("NULL Object! Can't process a scene!");
		}
	}
	
// 	pcstr start_scene		= (*m_game_project->m_config)["start"]["scene_name"];
// 	game_object_ptr_ s		= get_object_by_name(start_scene);
// 	object_scene_ptr scene	= static_cast_resource_ptr<object_scene_ptr>(s);
// 	scene->start			( );
}

void game_world::on_scene_start( object_scene_ptr scene )
{
	m_active_scenes.push_back	( scene );
}

void game_world::on_scene_stop( object_scene_ptr scene )
{
	scenes_list::iterator it	= std::find(m_active_scenes.begin(), m_active_scenes.end(), scene );
	R_ASSERT					( it!=m_active_scenes.end() );
	m_active_scenes.erase		( it );
}

void game_world::query_resources( )
{
	xray::render::scene_configuration				scene_configuration;
	scene_configuration.m_create_terrain			= true;
	scene_configuration.m_create_particle_world		= true;
	scene_configuration.m_create_speedtree_world	= true;
	scene_configuration.m_create_grass_world		= true;
	scene_configuration.m_sky_enabled				= true;
	
	resources::user_data_variant					scene_data;
	scene_data.set									( scene_configuration );

	
	xray::resources::user_data_variant const* data[] = { &scene_data, 0, 0  };

	xray::const_buffer			temp_buffer( "", 1 );

	xray::resources::creation_request requests[] = 
	{
		xray::resources::creation_request( "game_scene", temp_buffer, resources::scene_class ),
		xray::resources::creation_request( "game_scene_view", temp_buffer, resources::scene_view_class ),
		xray::resources::creation_request( "game_sound_scene", temp_buffer, resources::sound_scene_class ),
	};
 	xray::resources::query_create_resources(
 		requests,
		boost::bind( &game_world::on_resources_ready, this, _1 ),
 		stalker2::g_allocator,
 		data
 	);
}

void game_world::delete_actor() {
	if (m_local_actor) {
		m_local_actor->m_stop_query = true;
		DELETE(m_local_actor);
	}
}

void game_world::on_resources_ready( resources::queries_result& data )
{
	R_ASSERT			( data.is_successful( ) );

	m_scene				= static_cast_resource_ptr< xray::render::scene_ptr >( data[0].get_unmanaged_resource() );
	m_scene_view		= static_cast_resource_ptr< xray::render::scene_view_ptr >( data[1].get_unmanaged_resource() );

	m_sound_scene		= static_cast_resource_ptr< xray::sound::sound_scene_ptr >( data[2].get_unmanaged_resource() );
	
	if(is_active())
		get_game().get_sound_world().get_logic_world_user().set_active_sound_scene( get_sound_scene(), 1000, 0 );
}

void game_world::clear_resources( )
{
	get_game().get_sound_world().get_logic_world_user().remove_sound_scene	( get_sound_scene() );
}

void game_world::tmp_actor_ready( actor* a )
{
	get_game().gload = false;

	if (!m_actor_spawned)
		return;

	R_ASSERT				(a==m_local_actor);
	m_actor_input_controller->m_actor = m_local_actor;
	a->set_input_source		( m_actor_input_controller );

	float4x4 initial_matrix = float4x4().identity();
	initial_matrix.c		= m_camera_director->get_inverted_view_matrix().c;
	a->activate				( initial_matrix );

	m_camera_director->switch_to_camera	( m_actor_input_controller, "actor camera" );

//	volumetric_test = NEW(object_volumetric_sound)(*this);
//	volumetric_test->load_custom("rain");
	m_game.load_speedtree(&m_game.renderer());
}

} // namespace stalker2
