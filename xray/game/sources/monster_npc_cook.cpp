////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "monster_npc_cook.h"
#include "monster_npc.h"
#include "game.h"
#include <string>
#include <iostream>

namespace stalker2 {

monster_npc_cook::monster_npc_cook	( game& game ) :
	translate_query_cook		( resources::monster_npc_class, reuse_true, use_current_thread_id ),
	m_game						( game ),
	m_type						("")
{
}

//  void human_npc_cook::translate_request_path	( pcstr request, fs_new::virtual_path_string& new_request ) const
//  {
//  	new_request.assignf			( "resources/npc/human/%s.npc", request );
//  }

void monster_npc_cook::translate_query	( resources::query_result_for_cook& parent )
{
	configs::binary_config_value* t_object	= ( configs::binary_config_value* )( parent.creation_data_from_user().c_ptr() );

	if ( t_object != 0 )
	{
		pcstr npc_brain_config_path		= ( *t_object )["brain"];
		resources::query_resource		(
			npc_brain_config_path,
			resources::binary_config_class,
			boost::bind( &monster_npc_cook::on_queried_data_received, this, _1 ),
			g_allocator,
			parent.user_data(),
			&parent
		);
		return;
	}
		

	resources::query_resource			(
		parent.get_requested_path(),
		resources::binary_config_class,
		boost::bind( &monster_npc_cook::on_queried_data_received, this, _1 ),
		g_allocator,
		parent.user_data(),
		&parent
	);
}

void monster_npc_cook::delete_resource	( resources::resource_base* resource )
{
 	XRAY_DELETE_IMPL					( g_allocator, resource );
}

void monster_npc_cook::on_queried_data_received			( resources::queries_result& data )
{
	resources::query_result_for_cook* const	parent		= data.get_parent_query();
	if ( !data.is_successful() )
	{
 		R_ASSERT										( data.is_successful(), "couldn't retrieve npc config options" );
		parent->finish_query							( result_error );
		return;
	}

	configs::binary_config_ptr config					= static_cast_resource_ptr< configs::binary_config_ptr >( data[0].get_unmanaged_resource() );

	if (!config.c_ptr())
		return;

	on_npc_options_received								( config->get_root(), *parent );
}

void monster_npc_cook::on_npc_options_received			( configs::binary_config_value const& config_value , resources::query_result_for_cook& parent )
{
	configs::binary_config_value const& attributes		= config_value["attributes"];
	configs::binary_config_value* project_config		= ( configs::binary_config_value* )( parent.creation_data_from_user().c_ptr() );

	monster_npc* const monster					= XRAY_NEW_IMPL( g_allocator, monster_npc )(
												m_game.get_ai_world( ),
												m_game.get_sound_world( ),
												m_game.get_sound_scene( ),
												m_game.get_spatial_tree( ),
												m_game.get_active_scene( ),
												m_game.renderer( ),
												m_game.get_game_world( )
											);
	
	R_ASSERT								( attributes.value_exists( "brain_unit" ) );
	pcstr brain_unit_path					= attributes["brain_unit"];

	//R_ASSERT								( attributes.value_exists( "model" ) );
	//pcstr model_path						= attributes["model"];
	R_ASSERT								( project_config->value_exists( "model" ) );
	pcstr model_path						= (*project_config)["model"];

	m_type = model_path;


	ai::brain_unit_cook_params				cook_brain_unit_params;
	cook_brain_unit_params.world_user_type	= resources::sound_player_logic_class;

	cook_brain_unit_params.npc				= monster;
	resources::user_data_variant			brain_unit_params;
	brain_unit_params.set					( cook_brain_unit_params );
	
	resources::user_data_variant			physics_world;
	physics_world.set						( &m_game.physics() );

	if (m_type.find("bloodsucker") != std::string::npos) {
		resources::request requests[] =
		{
			{ brain_unit_path, resources::brain_unit_class },
			{ model_path, resources::game_animated_model_instance_class },
			{ "resources/animations/single/monster/bloodsucker/locomotion/stand_idle_1", resources::animation_class },
			{ "resources/animations/single/monster/bloodsucker/locomotion/walk_move_fwd_idle_1", resources::animation_class },
			{ "resources/animations/single/monster/bloodsucker/locomotion/walk_arc_fwd_left_idle_1", resources::animation_class },
			{ "resources/animations/single/monster/bloodsucker/locomotion/walk_arc_fwd_right_idle_1", resources::animation_class },
		};

		resources::user_data_variant const* params[] =
		{
			&brain_unit_params,
			&physics_world,
			0,
			0,
			0,
			0
		};

		query_resources(
			requests,
			array_size(requests),
			boost::bind(&monster_npc_cook::on_subresources_loaded, this, _1, monster),
			g_allocator,
			params,
			&parent
		);
	}
	else {
		resources::request requests[] =
		{
			{ brain_unit_path, resources::brain_unit_class },
			{ model_path, resources::game_animated_model_instance_class },
			{ "resources/animations/single/human/common_anim_slot_3/free/on_site_still_aim_1", resources::animation_class },
			{ "resources/animations/single/human/common_anim_slot_3/free/walk_move_fwd_aim_1", resources::animation_class },
			{ "resources/animations/single/human/common_anim_slot_3/free/walk_arc_fwd_left_aim_1", resources::animation_class },
			{ "resources/animations/single/human/common_anim_slot_3/free/walk_arc_fwd_right_aim_1", resources::animation_class },
			{ "ak_74",  resources::weapon_class }
		};

		resources::user_data_variant const* params[] =
		{
			&brain_unit_params,
			&physics_world,
			0,
			0,
			0,
			0,
			0
		};

		query_resources(
			requests,
			array_size(requests),
			boost::bind(&monster_npc_cook::on_subresources_loaded, this, _1, monster),
			g_allocator,
			params,
			&parent
		);
	}
}

void monster_npc_cook::on_subresources_loaded	( resources::queries_result& data, monster_npc* const monster )
{
	resources::query_result_for_cook* const	parent	= data.get_parent_query();

	configs::binary_config_value* monster_config = (configs::binary_config_value*)( parent->creation_data_from_user().c_ptr() );
	configs::binary_config_value monster_attributes_config = (*monster_config)["attributes"];

	if ( !data.is_successful() )
	{
		R_ASSERT							( data.is_successful(), "couldn't create some of npc subresources" );
		parent->finish_query				( result_error );
		return;
	}

	ai::brain_unit_res_ptr brain_unit_ptr	= static_cast_resource_ptr< ai::brain_unit_res_ptr >( data[0].get_unmanaged_resource() );
	monster->set_brain_unit					( brain_unit_ptr );

	animated_model_instance_ptr model_ptr	= static_cast_resource_ptr< animated_model_instance_ptr >( data[1].get_unmanaged_resource() );
	monster->set_model						( model_ptr );

	animation::skeleton_animation_ptr idle_animation		= static_cast_resource_ptr< animation::skeleton_animation_ptr >( data[2].get_managed_resource() );
	monster->set_idle_animation				( idle_animation );

	animation::skeleton_animation_ptr forward_animation		= static_cast_resource_ptr< animation::skeleton_animation_ptr >( data[3].get_managed_resource() );
	monster->set_walk_forward_animation		( forward_animation );

	animation::skeleton_animation_ptr arc_left_animation	= static_cast_resource_ptr< animation::skeleton_animation_ptr >( data[4].get_managed_resource() );
	monster->set_arc_left_animation			( arc_left_animation );

	animation::skeleton_animation_ptr arc_right_animation	= static_cast_resource_ptr< animation::skeleton_animation_ptr >( data[5].get_managed_resource() );
	monster->set_arc_right_animation			( arc_right_animation );

	if (m_type.find("bloodsucker") == std::string::npos) {
		monster->m_weapon						= static_cast_resource_ptr<weapon_ptr>(data[6].get_unmanaged_resource());
		monster->m_weapon->m_game_world			= &m_game.get_game_world();
		monster->m_weapon_bone_idx				= monster->m_model_instance->m_physics_model->m_skeleton->get_bone_index("Weapon")-1;
			//monster->m_model_instance->m_physics_model->m_skeleton->get_bone_index("RightHandMiddle_1")-1;
	}

	parent->set_unmanaged_resource			(
				monster, 
				resources::memory_usage_type( resources::nocache_memory, sizeof( monster_npc ) )
			);
	parent->finish_query					( result_success );

	m_game.on_monster_attributes_received		( monster_attributes_config, monster, m_type );
}

} // namespace stalker2
