////////////////////////////////////////////////////////////////////////////
//	Created		: 16.03.2012
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "lobby_menu.h"
#include "game.h"
#include <xray/input/world.h>
#include <xray/render/facade/common_types.h>
#include <xray/render/facade/scene_renderer.h>
#include "free_fly_camera.h"
#include "stats.h"

namespace stalker2{

lobby_menu::lobby_menu( game& w )
:super(w)
{
	//init_physics						( );
	m_camera_director					= NEW(camera_director)( *this );
	m_camera_director->set_no_delete	( );

	m_game_camera					= NEW(game_camera)(*this);

#ifndef MASTER_GOLD
	m_dbg_name							= "Lobby Menu";
#endif //#ifndef MASTER_GOLD

	m_camera_director->switch_to_camera	( m_game_camera, "free fly camera" );
 	math::float3 pos( 0.31f, 2.9f, 26.07f );
 	math::float3 dir( 0.9f, -0.22f, 0.37f );
 
 	m_game_camera->set_position_direction		( pos, dir );

	query_resources( );
}

lobby_menu::~lobby_menu( )
{
	DELETE				( m_game_camera );
	camera_director* cd = m_camera_director.c_ptr(); 
	m_camera_director	= NULL;
	DELETE				( cd );


	m_lobby_game_project = NULL;
}

void lobby_menu::clear_resources			( )
{
	vector<game_object_*>::iterator it = m_lobby_game_project->m_objects.begin();
	vector<game_object_*>::iterator it_e = m_lobby_game_project->m_objects.end();

	for ( ; it!=it_e; ++it )
	{
		game_object_* game_obj = *it;

		game_obj->unload_contents();
	}
}


void lobby_menu::on_activate( )
{
	//load scene objects
	super::on_activate					( );
	get_game().input_world().add_handler	( *this );
	m_camera_director->on_focus	( true );
}

void lobby_menu::on_deactivate( )
{
	super::on_deactivate				( );
	get_game().input_world().remove_handler	( *this );
	m_camera_director->on_focus	( false );
}

void lobby_menu::tick( )
{
	super::tick							( );
	m_camera_director->tick				( );
	m_camera_director->apply			( );
#ifndef MASTER_GOLD
	get_game().get_stats().set_camera_stats	( m_inverted_view_matrix.c.xyz(), m_inverted_view_matrix.k.xyz() );
#endif
}

void lobby_menu::query_resources( )
{
	render::scene_configuration				render_configuration;
	render_configuration.m_create_terrain			= false;
	render_configuration.m_create_particle_world	= true;
	render_configuration.m_sky_enabled				= true;
	
	resources::user_data_variant temp_data;
	temp_data.set				( render_configuration );

	resources::user_data_variant const* data[] = { &temp_data, 0, 0 };
	
	xray::const_buffer			temp_buffer( "", 1 );
	resources::creation_request requests[] = 
	{
		resources::creation_request( "game_scene", temp_buffer, resources::scene_class ),
		resources::creation_request( "game_scene_view", temp_buffer, resources::scene_view_class ),
		
	};
 	xray::resources::query_create_resources(
 		requests,
		boost::bind( &lobby_menu::on_render_scenes_ready, this, _1 ),
 		stalker2::g_allocator,
 		data
 	);

}


void lobby_menu::on_game_project_ready	( xray::resources::queries_result& data )
{
	m_lobby_game_project = static_cast_resource_ptr<simple_game_project_ptr>(data[0].get_unmanaged_resource());

	vector<game_object_*>::iterator it = m_lobby_game_project->m_objects.begin();
	vector<game_object_*>::iterator it_e = m_lobby_game_project->m_objects.end();

	for ( ; it!=it_e; ++it )
	{
		game_object_* game_obj = *it;

		game_obj->load_contents();
	}

}

void lobby_menu::on_render_scenes_ready( xray::resources::queries_result& data )
{
	m_scene				= static_cast_resource_ptr< render::scene_ptr >( data[0].get_unmanaged_resource() );
	m_scene_view		= static_cast_resource_ptr< render::scene_view_ptr >( data[1].get_unmanaged_resource() );
 

	resources::user_data_variant lobby_data;
	lobby_data.set				( this );


	xray::resources::query_resource(
		"lobby_scene",
		resources::game_project_simple_class ,
		boost::bind( &lobby_menu::on_game_project_ready, this, _1 ),
		g_allocator,
		&lobby_data
		);
	

// 	renderer().scene().add_model(	get_render_scene(), 
// 									m_static_model->m_render_model, 
// 									math::float4x4().identity() );

}





}
