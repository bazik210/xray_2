////////////////////////////////////////////////////////////////////////////
//	Created		: 28.07.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "actor.h"
#include <xray/animation/instant_interpolator.h>
#include <xray/animation/mixing_animation_lexeme.h>
#include <xray/animation/mixing_math.h>
#include <xray/animation/skeleton_animation.h>
#include <xray/render/facade/scene_renderer.h>
#include <xray/render/facade/debug_renderer.h>
#include "game_world.h"
#include "game.h"
#include <xray/physics/character_controller.h>
#include <xray/physics/world.h>
#include <xray/physics/rigid_body.h>
#include "collision_object_types.h"
#include <xray/animation/cubic_spline_skeleton_animation.h>
#include "actor_input_controller.h"
#include "weapon.h"

namespace stalker2{

actor::actor( game_world& w )
:m_character_transform( float4x4().identity() ),
m_look_pitch		( 0.0f ),
m_actor_input_controller( NULL ),
m_animation_player	( NULL ),
m_tmp_is_active		( false ),
m_game_world		( w )
{
	m_animation_player			= NEW(animation::animation_player)( );
	m_animation_player->set_no_delete();// ??

	m_actor_physics_controller	= xray::physics::create_character_controller(*g_allocator, m_game_world.get_physics_world() );
	m_actor_physics_controller->initialize( );
	m_anim_timer.start			( );

	query_resources				( );
}

actor::~actor( )
{
	m_actor_physics_controller->deactivate( );
	DELETE							( m_actor_physics_controller );
	m_actor_physics_controller		= NULL;
	remove_models_from_scene		( );

	m_character_model		= NULL;

	m_idle_stand_animation	= NULL;
	m_look_animation_add	= NULL;
	DELETE					( m_animation_player );
	m_animation_player		= NULL;

}

void actor::set_input_source( actor_input_controller* s )
{
	m_actor_input_controller = s;
}

void actor::query_resources( )
{
	resources::request r[] ={
		{ "character/human/neutral/neutral_01/neutral_01",		resources::skeleton_model_instance_class },
		{ "resources/animations/single/human/hud/stand_idle",	resources::animation_class },
		{ "resources/animations/single/human/hud/stand_add",	resources::animation_class },
		{ "ak_74",												resources::weapon_class },
	};

	resources::query_resources(
		r,
		boost::bind( &actor::on_resources_ready, this, _1 ),
		g_allocator
	);
}

void actor::on_resources_ready( resources::queries_result& data )
{
	if(!data.is_successful())
		return;

	m_character_model		= static_cast_resource_ptr<render::skeleton_model_ptr>(data[0].get_unmanaged_resource());

	m_idle_stand_animation	= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[1].get_managed_resource());
	m_look_animation_add	= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[2].get_managed_resource());

	m_head_bone_idx			= m_character_model->m_skeleton->get_bone_index("Head")-1;
	m_weapon_bone_idx		= m_character_model->m_skeleton->get_bone_index("Weapon")-1;

	m_weapon				= static_cast_resource_ptr<weapon_ptr>(data[3].get_unmanaged_resource());
	m_weapon->m_game_world	= &m_game_world;
	
	m_game_world.tmp_actor_ready( this );
}

void actor::add_models_to_scene( )
{
	render::scene_ptr scene		= m_game_world.get_render_scene();
	render::game::renderer& r	= m_game_world.renderer();

	r.scene().add_model			( scene, m_character_model->m_render_model, m_character_transform );
	m_weapon->show				( m_character_transform );
}

void actor::remove_models_from_scene( )
{
	render::scene_ptr scene		= m_game_world.get_render_scene();
	render::game::renderer& r	= m_game_world.renderer();

	r.scene().remove_model		( scene, m_character_model->m_render_model );
	m_weapon->hide				( );
}

void actor::activate( math::float4x4 const& initial_matrix )
{
	m_character_transform			= initial_matrix;

	m_weapon->action				( 1 );//shoot

	m_actor_physics_controller->activate		( m_character_transform );
	m_animation_player->set_object_transform	( m_character_transform );
	add_models_to_scene				( );
	m_tmp_is_active					= true;
}

collision::geometry_instance& actor::get_caracter_capsule( )
{
	return m_actor_physics_controller->get_capsule( );
}

void actor::process_input_events( )
{
	if(!m_actor_input_controller)
		return;

	float const angle_factor		= 0.5f;

	//1.0-up....-1.0f-down
	//m_look_pitch = 0.0f;//m_actor_input_controller->m_camera_inv_view_.k.xyz() | float3(0.0f, 1.0f, 0.0f);
	m_look_pitch		-= angle_factor*m_actor_input_controller->onframe_turn_x();
	math::clamp			(m_look_pitch, -0.98f, 0.98f);

	// apply rotation without physic simulation
	{
		float3 const angles_zxy			= m_character_transform.get_angles( math::rotation_zxy );
		float3 const new_angles_zxy		= float3(	angles_zxy.x, 
													angle_factor*m_actor_input_controller->onframe_turn_y() + angles_zxy.y, 
													angles_zxy.z );

		float4x4 rotation				= math::create_rotation( new_angles_zxy, math::rotation_zxy );
		float4x4 const translation		= math::create_translation( m_character_transform.c.xyz() );
		m_character_transform			= rotation * translation;
		m_actor_physics_controller->set_transform( m_character_transform );
	}

	// apply desired moving
	{
		float const frame_time_sec		= m_actor_input_controller->last_frame_time_delta()/1000.0f;
		
		float const move_delta_fw		= frame_time_sec * 1.66f *6; //6km/h
		float const move_delta_right	= frame_time_sec * 0.83f *6; //3km/h
		float3 walk_direction			= m_character_transform.k.xyz() * m_actor_input_controller->onframe_move_fwd() * move_delta_fw;
		walk_direction					+= m_character_transform.i.xyz() * m_actor_input_controller->onframe_move_right() * move_delta_right;

		m_actor_physics_controller->set_walk_direction( walk_direction  );
	}

	if(m_actor_input_controller->onframe_jump())
		m_actor_physics_controller->jump();
}

void actor::update_animations( )
{

	mutable_buffer buffer	( ALLOCA( animation::animation_player::stack_buffer_size ), animation::animation_player::stack_buffer_size );

	animation::skeleton_animation_ptr current_idle_animation		= m_idle_stand_animation;
	animation::skeleton_animation_ptr current_additive_animation	= m_look_animation_add;

	// caclulate additive animation coefficient, based on pitch
	float k								= 1.0f - (m_look_pitch+1.0f)/2.0f; // normalized to 0..1.0f
	float additive_current_anim_time	= animation::cubic_spline_skeleton_animation_pinned( current_additive_animation ).c_ptr()->length_in_frames() / animation::default_fps * k;

	animation::mixing::animation_lexeme	current_idle_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer, 
			"idle",
			current_idle_animation
		).time_scale( 0.f )
	);

	animation::mixing::animation_lexeme	current_additive_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer, 
			"additive",
			current_additive_animation
		)
		.time_scale( 0.f )
		.start_animation_interval_time( additive_current_anim_time )
		.override_existing_animation( true )
		.additivity_priority( 1 )
	);

	u32 current_time				= m_anim_timer.get_elapsed_msec();
	
	animation::mixing::animation_lexeme weapon_target = m_weapon->select_animation( buffer );

	m_animation_player->set_target_and_tick	( 
						current_idle_lexeme
						+ current_additive_lexeme
						+ weapon_target
						,current_time );

}

void actor::tick( )
{
	if(!m_tmp_is_active)
		return;

	// from previous physic step
	m_character_transform = m_actor_physics_controller->get_transform();

	process_input_events	( );

	update_animations		( );

	render::scene_ptr scene			= m_game_world.get_render_scene();
	render::game::renderer& r		= m_game_world.renderer();

	
	float4x4 const m				= create_rotation(float3(0.0f, math::pi, 0.0f)) * m_character_transform;
	
	r.scene().update_model			( scene, m_character_model->m_render_model, m );

	u32 const non_root_bones_count	= m_character_model->m_skeleton->get_non_root_bones_count( );
	float4x4* const matrices		= static_cast<float4x4*>( ALLOCA(non_root_bones_count*sizeof(float4x4)) );
	m_animation_player->compute_bones_matrices( *m_character_model->m_skeleton, matrices, matrices + non_root_bones_count );

	r.scene().update_skeleton		( m_character_model->m_render_model, matrices, non_root_bones_count );


	// update weapon
	{
		float4x4 weapon_matrix;
		calculate_weapon_matrix		( matrices, weapon_matrix );
		m_weapon->set_transform		( weapon_matrix );
		m_weapon->tick				( m_animation_player );
	}

	calculate_head_matrix		( matrices, m_character_head_transform );

	// other stuff (test, temp etc)
	{
		float3 ray_from		= m_character_head_transform.c.xyz();
		float3 ray_dir		= m_character_head_transform.k.xyz();
		float ray_length	= 100.0f;

		render::debug::renderer& d	= r.debug();

		physics::closest_ray_result result = m_game_world.get_physics_world()->ray_test( ray_from, ray_dir, ray_length );
		
		if(result.m_object)
		{
			d.draw_aabb( scene, result.m_hit_point_world, float3(0.01f,0.01f,0.01f), math::color(0,255,0,255));

			if(m_actor_input_controller && m_actor_input_controller->on_frame_fire())
			{
				// shooting
				// weapon snd(2d or 3d???)
				if(!result.m_object->is_static_or_kinematic_object())
				{
					// play shootmark snd 3d!!!
					float const impulse_strength	= 100.f;
					result.m_object->apply_impulse	( ray_dir*impulse_strength, result.m_hit_point_world );
				}
			}
		}
	}
}

void actor::calculate_head_matrix( float4x4* const matrices, float4x4& result ) const
{
	float4x4 character_render_transform		= create_rotation(float3(0.0f, math::pi, 0.0f)) * m_character_transform;
	result							= ( create_rotation(float3(0,0,math::pi_d2)) * 
											matrices[m_head_bone_idx] * 
											character_render_transform );

	result.c.xyz()			+= result.j.xyz()*0.1f;
}

void actor::calculate_weapon_matrix( float4x4* const matrices, float4x4& result  ) const
{
	float4x4 character_render_transform	= create_rotation(float3(0.0f, math::pi, 0.0f)) * m_character_transform;
	result								= matrices[m_weapon_bone_idx] * character_render_transform;
}

}
