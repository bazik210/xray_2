////////////////////////////////////////////////////////////////////////////
//	Created		: 21.03.2012
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game.h"
/*
void game::initialize_rtp			( )
{
	ASSERT							( !m_initialized );
	ASSERT							( m_animation_world );
	ASSERT							( !m_rtp_world );
	m_rtp_world						= rtp::create_world( *this, *m_animation_world, m_scene, &m_renderer, m_engine.get_resources_path() );

	game_test_suite::singleton()->set_rtp_world( m_rtp_world );

	ASSERT							( m_rtp_world );
}


void game::initialize_ai_navigation	( )
{
	ASSERT							( !m_initialized );
	ASSERT							( !m_ai_navigation_world );
	//m_ai_navigation_world			= ai::navigation::create_world( *this, m_scene, m_renderer.debug() );
	ASSERT							( m_ai_navigation_world );
}

void game::initialize_ai			( )
{
	ASSERT							( !m_initialized );

	ASSERT							( !m_spatial_tree );
	m_spatial_tree					= &*collision::new_space_partitioning_tree( g_allocator, 1.f, 1024 );
	ASSERT							( m_spatial_tree );
	
	ASSERT							( !m_ai_world );
	m_ai_world						= ai::create_world( *this );
	ASSERT							( m_ai_world );
}
static void delete_weapons				( human_npc_ptr& owner )
{
	while ( object_weapon* weapon = owner->pop_weapon() )
		DELETE							( weapon );
}

static void kill_npc					( human_npc_ptr& condemned )
{
	delete_weapons						( condemned );
	condemned->clear_resources			( );
}

void game::get_colliding_objects	( math::aabb const& query_aabb, xray::ai::ai_objects_type& results )
{
	xray::collision::objects_type	objects( g_allocator );
	m_spatial_tree->aabb_query		( collision_object_type_ai, query_aabb, objects );
	for ( xray::collision::objects_type::const_iterator iter = objects.begin(); iter != objects.end(); ++iter )
	{
		ai_collision_object const* const ai_object = static_cast_checked< ai_collision_object const* >( *iter );
		results.push_back			( &ai_object->get_game_object() );
	}
}

void game::get_visible_objects		( math::cuboid const& cuboid, xray::ai::update_frustum_callback_type const& update_callback )
{
	xray::collision::results_callback_type result_callback( boost::bind( &game::get_frustum_objects_callback, this, &update_callback, _1 ) );
	m_spatial_tree->cuboid_query	( collision_object_type_ai, cuboid, result_callback );
}

void game::get_frustum_objects_callback	(
		xray::ai::update_frustum_callback_type const* update_callback,
		xray::collision::object const& frustum_object
	)
{
	ai_collision_object const& ai_object = static_cast_checked< ai_collision_object const& >( frustum_object );
	( *update_callback )			( ai_object.get_game_object() );
}
bool game::ray_query				(
		ai::collision_object const* const object_to_pick,
		ai::collision_object const* const object_to_ignore,
		float3 const& origin,
		float3 const& direction,
		float const max_distance,
		float const transparency_threshold,
		float& visibility_value
	) const
{
	collision::ray_triangles_type game_objects = collision::ray_triangles_type( g_allocator );
	ray_query_predicate query_predicate = ray_query_predicate(
		visibility_value,
		static_cast_checked< ai_collision_object const* >( object_to_pick ),
		static_cast_checked< ai_collision_object const* >( object_to_ignore ),
		transparency_threshold
	);
	m_spatial_tree->ray_query( collision_object_type_ai, origin, direction, max_distance, game_objects, collision::triangles_predicate_type( &query_predicate, &ray_query_predicate::predicate ) );
	return query_predicate.requested_object_was_found;
}

void game::get_available_weapons( ai::npc* owner, ai::weapons_list& list_to_be_filled ) const
{
 	human_npc* const npc_owner	= static_cast_checked< human_npc* >( owner );
	npc_owner->get_available_weapons( list_to_be_filled );
}

void game::run_ai_tests		( u32 const current_frame_id )
{
	for ( human_npc_ptr it_npc = m_npcs.front(); it_npc; it_npc = m_npcs.get_next_of_object( it_npc ) )
		it_npc->tick		( current_frame_id );
}

void game::check_selected_npc			( )
{
	m_active_npc_set					= !m_active_npc_set;
	
	if ( m_active_npc_set )
		m_selected_npc					= find_npc_in_camera_direction();
	else
		m_selected_npc					= 0;

	if ( m_selected_npc == 0 )
		m_active_npc_set				= false;
}
void game::setup_movement_target		( )
{
	collision::ray_objects_type			objects( g_allocator );
	m_game_world->get_collision_tree()->ray_query(
		collision_object_type_terrain,
		get_camera_position(),
		m_inverted_view_matrix.k.xyz(),
		1000.f,
		objects,
		collision::objects_predicate_type()
	);

	if ( !objects.empty() )
		m_movement_target				= get_camera_position() + m_inverted_view_matrix.k.xyz() * objects.front().distance;
}

void game::toggle_npc_creation_mode		( )
{
	m_is_npc_auto_creation_enabled		= !m_is_npc_auto_creation_enabled;
}

struct get_first_npc_in_camera_direction_predicate : private boost::noncopyable
{
	inline get_first_npc_in_camera_direction_predicate	( ) :
		first_npc										( 0 )
	{
	}

	inline bool predicate					( xray::collision::ray_triangle_result const& triangle )
	{
		collision::object const* object_at_direction	= triangle.object;
		ai_collision_object const* ai_collision			= static_cast_checked< ai_collision_object const* >( object_at_direction );
		R_ASSERT							( ai_collision );
		first_npc							= ai_collision->get_game_object().cast_npc();
		return								first_npc != 0;
	}

	ai::npc* 								first_npc;
}; // struct get_first_npc_in_camera_direction_predicate

human_npc* game::find_npc_in_camera_direction	( ) const
{
	collision::ray_triangles_type game_objects	( g_allocator );
	get_first_npc_in_camera_direction_predicate query_predicate;
	m_spatial_tree->ray_query				(
		collision_object_type_ai,
		get_camera_position(),
		m_inverted_view_matrix.k.xyz(),
		50000.f,
		game_objects,
		collision::triangles_predicate_type( &query_predicate, &get_first_npc_in_camera_direction_predicate::predicate )
	);
	return query_predicate.first_npc ? static_cast_checked< human_npc* >( query_predicate.first_npc ) : 0;
}
void game::update_npc_stats				( )
{
	if ( m_active_npc_set && m_selected_npc )
	{
		m_active_npc_stats->set_stats	( m_selected_npc.c_ptr() );
		m_active_npc_stats->draw		( ui_world().get_renderer(), m_scene_view );
	}
}

void game::set_navmesh_info				( pcstr text ) const
{
	m_stats->set_navmesh_info			( text );
}

void game::query_npc_dictionary			( )
{
	query_resource	(
		"resources/brain_units/filters/available_filter_names.dictionary", 
		resources::binary_config_class, 
		boost::bind( &game::on_npc_dictionary_created, this, _1 ), 
		g_allocator
	);
}
void game::on_npc_dictionary_created	( resources::queries_result& data )
{
	if ( !data.is_successful() )
	{
		R_ASSERT						( data.is_successful(), "couldn't retrieve npc dictionary options" );
		return;
	}
		
	configs::binary_config_ptr config	= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource() );
	configs::binary_config_value const& dictionary = config->get_root();
	m_ai_world->fill_objects_dictionary	( dictionary );
	
	m_is_dictionary_created				= true;
}

void game::query_npc					( human_npc_behaviour_type_enum const behaviour_type, float3 const& initial_position )
{
	if ( !m_is_dictionary_created )
	{
		LOG_WARNING						( "dictionary wasn't created yet, so npc couldn't be created as well" );
		return;
	}
	
	if ( m_npc_queries_count == m_ai_world->get_characters_count() )
	{
		LOG_WARNING						( "limit of test characters is reached" );
		return;
	}

	pcstr config_name					= behaviour_type == human_npc_aggressive ? "aggressive" : "neutral";
	
	ai::brain_unit_cook_params			brain_unit_params;
	brain_unit_params.world_user_type	= resources::sound_player_logic_class;
		
	npc_cook_params						human_npc_params;
	human_npc_params.brain_unit_params	= brain_unit_params;
	human_npc_params.scene				= m_scene;
	human_npc_params.sound_scene		= m_sound_scene;
	human_npc_params.physics_world		= m_physics_world;
	resources::user_data_variant		npc_data;
	npc_data.set						( human_npc_params );

	query_resource						(
		config_name, 
		resources::monster_npc_class, 
		boost::bind( &game::on_npc_created, this, _1, initial_position ), 
		g_allocator,
		&npc_data,
		0
	);

	++m_npc_queries_count;
}

bool game::is_npc_id_available			( u32 const npc_id ) const
{
	for ( human_npc_ptr it_npc = m_npcs.front(); it_npc; it_npc = m_npcs.get_next_of_object( it_npc ) )
		if ( it_npc->get_id() == npc_id )
			return						false;

	return								true;
}

static void generate_weapons			(
		math::random32& randomizer,
		human_npc::npc_game_attributes& attributes,
		ai::world const& world,
		ai::weapon_types_enum weapon_type
	)
{
	u32 const random_id					= randomizer.random( world.get_weapons_count( weapon_type ) );
	attributes.weapons.push_back		( NEW( object_weapon )( weapon_type, world.get_weapon_name_by_id( weapon_type, random_id ), random_id ) );
}

void game::fill_npc_attributes_randomly	( human_npc_ptr owner, float3 const& initial_position )
{
	human_npc::npc_game_attributes		attributes;
	math::random32 randomizer			( m_ai_world->get_current_time_in_ms() );
	attributes.group_id					= randomizer.random( m_ai_world->get_groups_count() );
	attributes.outfit_id				= randomizer.random( m_ai_world->get_outfits_count() );
	attributes.debug_draw_color			= math::color( randomizer.random( 255 ), randomizer.random( 255 ), randomizer.random( 255 ) );
	attributes.initial_velocity			= randomizer.random_f( 16 );
	attributes.initial_position			= initial_position;
	u32 const weapons_count				= randomizer.random( 6 );
	for ( u32 i = 0; i < weapons_count; ++i )
	{
		ai::weapon_types_enum const weapon_type	= (ai::weapon_types_enum)randomizer.random( ai::weapon_types_count );
		generate_weapons				( randomizer, attributes, *m_ai_world, weapon_type );
	}

	u32 const characters_count			= m_ai_world->get_characters_count();
	u32 character_index					= randomizer.random( characters_count );
	ai::character_type name_id			= m_ai_world->get_character_attributes_by_index( character_index );
	while ( !is_npc_id_available( name_id.second ) )
	{
		++character_index				%= characters_count;
		name_id							= m_ai_world->get_character_attributes_by_index( character_index );
	}

	attributes.name						= name_id.first;
	attributes.id						= name_id.second;

	finish_npc_creation					( owner, attributes );
}

void game::fill_npc_attributes_manually	( human_npc_ptr owner )
{
	query_resource						(
		"resources/npc/human/game_attributes/for_manual_creation.attributes", 
		resources::binary_config_class, 
		boost::bind( &game::on_queried_npc_attributes_received, this, _1, owner ), 
		g_allocator
	);
}

void game::on_npc_attributes_received	( configs::binary_config_value const& attributes_config, human_npc_ptr owner )
{

	human_npc::npc_game_attributes		attributes;
	attributes.group_id					= attributes_config["group_id"];
	attributes.class_id					= attributes_config["class_id"];
	attributes.outfit_id				= attributes_config["outfit_id"];
	float3 color						= (float3)attributes_config["debug_draw_color"];
	attributes.debug_draw_color			= math::color( (u32)color.x, (u32)color.y, (u32)color.z );
	attributes.initial_velocity			= attributes_config["initial_velocity"];
	attributes.initial_luminosity		= attributes_config["initial_luminosity"];
	attributes.description				= attributes_config["description"];
	attributes.initial_position			= (float3)attributes_config["initial_position"];
	attributes.initial_rotation			= (float3)attributes_config["initial_rotation"];
	attributes.initial_scale			= (float3)attributes_config["initial_scale"];
	attributes.name						= attributes_config["name"];
	attributes.id						= attributes_config["id"];
	configs::binary_config_value const& weapons = attributes_config["weapons"];

	configs::binary_config_value::const_iterator it		= weapons.begin();
	configs::binary_config_value::const_iterator it_end = weapons.end();

	for ( ; it != it_end; ++it )
	{
		configs::binary_config_value const& gun		= *it;
		u32 const type								= gun["type"];
		ai::weapon_types_enum const weapon_type		= (ai::weapon_types_enum)type;
		u32 const weapon_id							= gun["id"];
		attributes.weapons.push_back				( NEW( object_weapon )( weapon_type, m_ai_world->get_weapon_name_by_id( weapon_type, weapon_id ), weapon_id ) );
	}

	finish_npc_creation					( owner, attributes );
}

void game::on_queried_npc_attributes_received	( resources::queries_result& data, human_npc_ptr owner )
{
	if ( !data.is_successful() )
	{
		R_ASSERT						( data.is_successful(), "couldn't retrieve npc attributes for manual creation" );
		return;
	}
		
	configs::binary_config_ptr config	= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource() );
	configs::binary_config_value const& config_root = config->get_root();

	on_npc_attributes_received			( config_root, owner );
}

void game::on_npc_created				( resources::queries_result& data, float3 const camera_position )
{
	R_ASSERT							( data.is_successful() );

	human_npc_ptr new_npc				= static_cast_resource_ptr< human_npc_ptr >( data[0].get_unmanaged_resource() );

	if ( m_is_npc_auto_creation_enabled )
		fill_npc_attributes_randomly	( new_npc, camera_position );
	else
		fill_npc_attributes_manually	( new_npc );

	if ( m_sound_test_allowed )
		new_npc->set_sound_dbg_mode		( true );
}

void game::finish_npc_creation			( human_npc_ptr& new_npc, human_npc::npc_game_attributes& attributes )
{
	new_npc->set_attributes				( attributes );
	new_npc->enable						( );
	m_npcs.push_back					( new_npc );
}

void game::rotate_selected_npc			( float const y_angle )
{
	if ( m_selected_npc )
	{
		float const y_angle_rad			= math::deg2rad( y_angle );
		float4x4 const& rotation		= math::create_rotation( m_selected_npc->get_rotation_angles() );
		float4x4 const& new_rotation	= math::create_rotation( float3( 0.f, y_angle_rad, 0.f ) );
		m_selected_npc->set_rotation	( rotation * new_rotation );
	}
}

void game::delete_selected_npc			( )
{
	if ( m_selected_npc )
	{
		m_npcs.erase					( m_selected_npc );
		kill_npc						( m_selected_npc );
		m_selected_npc					= 0;
		m_active_npc_set				= false;
		--m_npc_queries_count;
	}
}
void game::assign_behaviour				( )
{
	if ( m_selected_npc )
	{
		xray::ai::behaviour_cook_params	behaviour_params;
		resources::user_data_variant	new_params;
		new_params.set					( behaviour_params );
		
		query_resource					(
			"test",
			resources::behaviour_class,
			boost::bind( &game::on_behaviour_created, this, _1 ), 
			g_allocator,
			&new_params,
			0
		);
	}
}

void game::on_behaviour_created			( resources::queries_result& data )
{
	R_ASSERT							( data.is_successful() );

	resources::unmanaged_resource_ptr new_behaviour = data[0].get_unmanaged_resource();
	
	if ( m_selected_npc )
		m_selected_npc->set_behaviour	( new_behaviour );
}


*/