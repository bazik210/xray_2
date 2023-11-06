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

#include <xray/console_command.h>

namespace stalker2{

static bool g_thirdperson_value = false;
console_commands::cc_bool g_thirdperson("thirdperson", g_thirdperson_value, false, console_commands::command_type_user_specific);

actor::actor( game_world& w )
:m_character_transform( float4x4().identity() ),
m_look_pitch		( 0.0f ),
m_actor_input_controller( NULL ),
m_animation_player	( NULL ),
m_tmp_is_active		( false ),
m_stop_query		(false ),
m_game_world		( w ),
m_wpn_reload		( false ),
m_wpn_shoot			( false ),
m_wpn_draw			( false ),
m_wpn_holster		( false ),
m_wpn_hidden_1		( false ),
m_wpn_hidden_2		( false ),
m_snd				( 0 ),
m_start_reload_timer ( 0 ),
m_start_shoot_timer  ( 0 ),
m_start_draw_timer	 ( 0 ),
m_start_holster_timer ( 0 ),
m_switch_snd_time_delay ( 0 ),
m_switch_snd_time		( 0 ),
m_wpn_switch		( false ),
m_wpn_call			( false ),
m_new_weapon		("ak_74"),
m_new_anim			("resources/animations/single/weapons/assault_rifles/ak74m/1st_person/danger/player/reload")
{
	m_animation_player			= NEW(animation::animation_player)( );
	m_animation_player->set_no_delete();// ??

	m_actor_physics_controller	= xray::physics::create_character_controller(*g_allocator, m_game_world.get_physics_world() );
	m_actor_physics_controller->initialize( );
	m_anim_timer.start			( );

	query_resources				( );

	m_health = 100.f;
}

actor::~actor( )
{
		m_actor_physics_controller->deactivate();
		DELETE(m_actor_physics_controller);
		m_actor_physics_controller = NULL;

		if (m_character_model)
			remove_models_from_scene();
	
		m_character_model = NULL;

		m_idle_stand_animation = NULL;
		m_look_animation_add = NULL;

		DELETE(m_animation_player);
		m_animation_player = NULL;
}

void actor::set_input_source( actor_input_controller* s )
{
	m_actor_input_controller = s;
}

void actor::query_resources( )
{
	resources::request r[] ={
		{ "character/human/actor/neutral_03/neutral_03_actor_full",	resources::skeleton_model_instance_class },
		{ "resources/animations/single/human/actor/locomotion/stand/on_site_idle",	resources::animation_class },
		{ "resources/animations/single/human/actor/locomotion/stand/on_site_idle_01",  resources::animation_class },
		{ "resources/animations/single/human/actor/locomotion/stand/on_site_add",	resources::animation_class },
		{ "resources/animations/single/weapons/assault_rifles/ak74m/1st_person/danger/player/reload_empty", resources::animation_class },
		{ "resources/animations/single/weapons/assault_rifles/ak74m/1st_person/danger/player/fire_1",  resources::animation_class },
		{ "resources/animations/single/weapons/assault_rifles/ak74m/1st_person/danger/player/draw",  resources::animation_class },
		{ "resources/animations/single/weapons/assault_rifles/ak74m/1st_person/danger/player/holster",  resources::animation_class },
		{ "assault_rifles/ak74m",								resources::weapon_class },
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

	if (m_stop_query) {
		data.empty();
		return;
	}

	m_character_model		= static_cast_resource_ptr<render::skeleton_model_ptr>(data[0].get_unmanaged_resource());

	m_idle_stand_animation	= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[1].get_managed_resource());
	m_idle_stand_01_animation	= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[2].get_managed_resource());
	m_look_animation_add	= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[3].get_managed_resource());
	m_reload_animation		= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[4].get_managed_resource());
	m_shoot_animation		= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[5].get_managed_resource());
	m_draw_animation		= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[6].get_managed_resource());
	m_holster_animation		= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[7].get_managed_resource());

	m_head_bone_idx			= m_character_model->m_skeleton->get_bone_index("Head")-1;
	m_camera_bone_idx		= m_character_model->m_skeleton->get_bone_index("Camera_Root")-1;
	m_weapon_bone_idx		= m_character_model->m_skeleton->get_bone_index("Weapon")-1;

	m_weapon				= static_cast_resource_ptr<weapon_ptr>(data[8].get_unmanaged_resource());
	m_weapon->m_game_world	= &m_game_world;

	//some query system bug;
	m_stop_query = true;

	m_game_world.tmp_actor_ready( this );
}

void actor::add_models_to_scene( )
{
	render::scene_ptr scene		= m_game_world.get_render_scene();
	render::game::renderer& r	= m_game_world.renderer();

	r.scene().add_model			( scene, m_character_model->m_render_model, m_character_transform );
	m_weapon->show				( m_character_transform );
	m_wpn_draw = true;
}

void actor::remove_models_from_scene( )
{
	render::scene_ptr scene		= m_game_world.get_render_scene();
	render::game::renderer& r	= m_game_world.renderer();

	r.scene().remove_model		( scene, m_character_model->m_render_model );

	if(m_weapon && !m_weapon->m_hidden)
		m_weapon->hide				( );
}

void actor::activate( math::float4x4 const& initial_matrix )
{
	m_character_transform			= initial_matrix;

	m_weapon->action				( 0 );

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
		

		// kirill:	do smth with player speed
		//			- fps affect on it
		//			- hardcode bad, mkay

		float move_delta_fw		= frame_time_sec * 1.66f * 4.f;
		float move_delta_right	= frame_time_sec * 0.83f * 4.f;
		if (m_actor_input_controller->on_frame_sprint())
		{
			move_delta_fw		= frame_time_sec * 1.66f * 10.f;
			move_delta_right	= frame_time_sec * 0.83f * 10.f;

			// reset if unpressed key
			if (!m_actor_input_controller->is_doing_movement())
				m_actor_input_controller->m_sprint_toggle = false;
		}


		float3 walk_direction			= m_character_transform.k.xyz() * m_actor_input_controller->onframe_move_fwd() * move_delta_fw;
		walk_direction					+= m_character_transform.i.xyz() * m_actor_input_controller->onframe_move_right() * move_delta_right;

		m_actor_physics_controller->set_walk_direction( walk_direction  );
	}

	if(m_actor_input_controller->onframe_jump())
		m_actor_physics_controller->jump();

	//pressed reload action
	if (m_actor_input_controller && m_actor_input_controller->m_reload && !m_wpn_reload && !m_wpn_switch && !m_wpn_holster && !m_wpn_draw && !m_wpn_hidden_1 && !m_wpn_hidden_2) 
	{
		m_actor_input_controller->m_reload = !m_actor_input_controller->m_reload;
		m_weapon->action				( 2 );
		update_animations(true, false, false, false, false);
		m_wpn_reload = true;
	}

	//pressed 1
	else if (m_actor_input_controller && m_actor_input_controller->m_wpn_1 && !m_wpn_reload && !m_wpn_holster && !m_wpn_draw && !m_wpn_switch)
	{
		m_actor_input_controller->m_wpn_1 = !m_actor_input_controller->m_wpn_1;

		if (strcmp(m_new_weapon,"ak_74")) 
		{
			m_wpn_switch = true;
			m_wpn_timer = m_anim_timer.get_elapsed_msec() + 100;

		}
		else {
			if (!m_wpn_hidden_1) {
				m_wpn_hidden_1 = true;
				m_wpn_holster = true;
			}
			else {
				m_wpn_hidden_1 = false;
				m_wpn_draw = true;
				m_weapon->show(m_character_transform);
			}
		}
	}

	//pressed 2
	else if (m_actor_input_controller && m_actor_input_controller->m_wpn_2 && !m_wpn_reload && !m_wpn_holster && !m_wpn_draw && !m_wpn_switch)
	{
		m_actor_input_controller->m_wpn_2 = !m_actor_input_controller->m_wpn_2;

		if (strcmp(m_new_weapon,"assault_rifles/ak74m")) 
		{
			m_wpn_switch = true;
			m_wpn_timer = m_anim_timer.get_elapsed_msec() + 100;
		}
		else {
			if (!m_wpn_hidden_2) {
				m_wpn_hidden_2 = true;
				m_wpn_holster = true;
			}
			else {
				m_wpn_hidden_2 = false;
				m_wpn_draw = true;
				m_weapon->show(m_character_transform);
			}
		}
	}
	m_actor_input_controller->m_reload = false;
	m_actor_input_controller->m_wpn_1 = false;
	m_actor_input_controller->m_wpn_2 = false;
}

void actor::query_new_weapon( pstr weapon_type, pstr new_anim )
{
	resources::request r[] ={
		{ weapon_type,										resources::weapon_class },
		{ new_anim,										resources::animation_class },
	};

	resources::query_resources(
		r,
		boost::bind( &actor::on_weapon_loaded, this, _1 ),
		g_allocator
	);
}

void actor::on_weapon_loaded(resources::queries_result& data)
{
	R_ASSERT(data.is_successful());

	if (m_weapon && !m_weapon->m_hidden) {
		if (!m_wpn_hidden_1 && !m_wpn_hidden_2) {
			m_weapon->hide();
		}
	}

	m_weapon = static_cast_resource_ptr< weapon_ptr >(data[0].get_unmanaged_resource());
	m_weapon->m_game_world	= &m_game_world;

	m_reload_animation		= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[1].get_managed_resource());

	m_weapon->action				( 0 );

	if (!strcmp(m_new_weapon,"ak_74")) 
	{
		m_new_weapon = "assault_rifles/ak74m";
		m_new_anim = "resources/animations/single/weapons/assault_rifles/ak74m/1st_person/danger/player/reload";
	}
	else if (!strcmp(m_new_weapon,"assault_rifles/ak74m"))
	{
		m_new_weapon = "ak_74";
		//m_new_anim = "resources/animations/single/human/hud/reload";
		//old anim doesn't work
	}

	m_wpn_draw = true;
	update_animations(false, false, true, false, false);

	m_weapon->show(m_character_transform);
}

void actor::switch_weapon() {
	query_new_weapon(m_new_weapon, m_new_anim);
}

//animation::callback_return_type_enum actor::on_animation_end(
//	animation::skeleton_animation_ptr const& ended_animation,
//	pcstr const subscribed_channel,
//	u32 const callback_time_in_ms,
//	u32 const domain_data
//) 
//{
//	m_wpn_reload = false;
//	m_start_reload_timer = false;

//	//m_animation_player->unsubscribe( xray::animation::channel_id_on_animation_end, 0 );

//	return							animation::callback_return_type_dont_call_me_anymore;
//}

void actor::update_animations( bool m_reload = false, bool m_shoot = false, bool m_draw = false, bool m_holster = false, bool m_idle = false )
{

	mutable_buffer buffer	( ALLOCA( animation::animation_player::stack_buffer_size ), animation::animation_player::stack_buffer_size );

	animation::skeleton_animation_ptr current_idle_animation		= m_idle_stand_animation;
	animation::skeleton_animation_ptr current_idle_01_animation		= m_idle_stand_01_animation;
	animation::skeleton_animation_ptr current_additive_animation	= m_look_animation_add;
	animation::skeleton_animation_ptr current_reload_animation		= m_reload_animation;
	animation::skeleton_animation_ptr current_shoot_animation		= m_shoot_animation;
	animation::skeleton_animation_ptr current_draw_animation		= m_draw_animation;
	animation::skeleton_animation_ptr current_holster_animation		= m_holster_animation;

	// calculate additive animation coefficient, based on pitch
	float k								= 1.0f - (m_look_pitch+1.0f)/2.0f; // normalized to 0..1.0f
	float additive_current_anim_time	= animation::cubic_spline_skeleton_animation_pinned( current_additive_animation ).c_ptr()->length_in_frames() / animation::default_fps * k;

	animation::mixing::animation_lexeme	current_idle_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer, 
			"idle",
			current_idle_animation
		).time_scale( 1.f )
	);

	animation::mixing::animation_lexeme	current_idle_no_wpn_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer, 
			"no_wpn",
			current_idle_01_animation
		).time_scale( 1.f )
	);

	animation::mixing::animation_lexeme	current_reload_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			"reload",
			current_reload_animation
		).time_scale(1.f)
	);

		animation::mixing::animation_lexeme	current_shoot_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			"shoot",
			current_shoot_animation
		).time_scale(1.f)
	);

	animation::mixing::animation_lexeme	current_draw_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			"draw",
			current_draw_animation
		).time_scale(1.f)
	);

	animation::mixing::animation_lexeme	current_holster_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			"holster",
			current_holster_animation
		).time_scale(1.f)
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

	m_weapon->select_animation(buffer);

	if (!m_reload && !m_shoot && !m_draw && !m_holster && !m_idle) {
		m_animation_player->set_target_and_tick(
			current_idle_lexeme
			+ current_additive_lexeme
			+ weapon_target
			, current_time);
	}
	else {
		if (m_reload) {
			if (!m_start_reload_timer) {
				m_start_reload_timer = true;
				auto m_snd = NEW(object_volumetric_sound)(m_game_world);
				m_snd->load_custom("reload",  m_character_transform, false);
				m_reload_anim_time = current_time + (current_reload_lexeme.animation_intervals_begin()->length() * 1000);
			}
			m_animation_player->set_target_and_tick(
				current_reload_lexeme
				+ current_additive_lexeme
				+ weapon_target
				, current_time);
			//	m_animation_player->subscribe(
			//		xray::animation::channel_id_on_animation_end,
			//		boost::bind(&actor::on_animation_end, this, _1, _2, _3, _4),
			//		0
			//	);
		}
		else if (m_shoot) {
			if (!m_start_shoot_timer) {
				m_start_shoot_timer = true;
				auto m_snd = NEW(object_volumetric_sound)(m_game_world);
				m_snd->load_custom("shoot",  m_character_transform, false);
				m_shoot_anim_time = current_time + (current_shoot_lexeme.animation_intervals_begin()->length() * 1000);
			}
			m_animation_player->set_target_and_tick(
				current_shoot_lexeme
				+ current_additive_lexeme
				+ weapon_target
				, current_time);
		}
		else if (m_draw) {
			if (!m_start_draw_timer) {
				m_start_draw_timer = true;
				auto m_snd = NEW(object_volumetric_sound)(m_game_world);
				m_snd->load_custom("draw", m_character_transform, false);
				m_draw_anim_time = current_time + (current_draw_lexeme.animation_intervals_begin()->length() * 1000);
			}
			m_animation_player->set_target_and_tick(
				current_draw_lexeme
				+ current_additive_lexeme
				+ weapon_target
				, current_time);
		}
		else if (m_holster) {
			if (!m_start_holster_timer) {
				m_start_holster_timer = true;
				auto m_snd = NEW(object_volumetric_sound)(m_game_world);
				m_snd->load_custom("holster",  m_character_transform, false);
				m_holster_anim_time = current_time + (current_holster_lexeme.animation_intervals_begin()->length() * 1000);
			}
			m_animation_player->set_target_and_tick(
				current_holster_lexeme
				+ current_additive_lexeme
				+ weapon_target
				, current_time);
		}
		else if (m_idle) {
			m_animation_player->set_target_and_tick(
				current_idle_no_wpn_lexeme
				+ current_additive_lexeme
				, current_time);
		}
		//m_animation_player->tick(current_time);
	}
}

void actor::tick()
{
	if (!m_tmp_is_active)
		return;

	// from previous physic step
	if (!m_actor_physics_controller)
		return;

	//if reload, wait till anim ends
	if (m_start_reload_timer && m_anim_timer.get_elapsed_msec() >= m_reload_anim_time) {
		m_start_reload_timer = false;
		m_weapon->action(0);
		m_wpn_reload = false;
	}

	if (m_start_draw_timer && m_anim_timer.get_elapsed_msec() >= m_draw_anim_time) {
		m_start_draw_timer = false;
		m_wpn_draw = false;

		if (m_wpn_switch) {
			m_wpn_switch = false;
			m_wpn_call = false;
			if (m_wpn_hidden_1)
				m_wpn_hidden_1 = false;

			if (m_wpn_hidden_2)
				m_wpn_hidden_2 = false;
		}

	}

	if (m_switch_snd_time_delay && m_anim_timer.get_elapsed_msec() >= m_switch_snd_time)
	{
		m_switch_snd_time_delay = false;

		if (m_wpn_switch) {
			switch_weapon();
		}
	}

	if (m_start_holster_timer && m_anim_timer.get_elapsed_msec() >= m_holster_anim_time) {
		m_start_holster_timer = false;
		m_wpn_holster = false;

		if (m_weapon && !m_weapon->m_hidden && !m_wpn_switch) {
			m_weapon->hide();
		}
		else {
			m_switch_snd_time = m_anim_timer.get_elapsed_msec() + 400;
			m_switch_snd_time_delay = true;

		}

	}

	m_character_transform = m_actor_physics_controller->get_transform();

	process_input_events();

	if (m_weapon) {
		if (!m_weapon->m_hidden) {
			if (!m_wpn_switch && !m_wpn_draw && !m_wpn_holster) {
				if (m_wpn_reload) {
					update_animations(true, false);
				}
				else if (m_wpn_shoot) {
					update_animations(false, true);
				}
				else {
					update_animations();

				}
			}
			if (m_wpn_draw) {
				update_animations(false, false, true, false);
			}
			else if (m_wpn_holster) {
				update_animations(false, false, false, true);
			}
			//else {
				//update_animations();
			//}
		}
		else {
			update_animations(false, false, false, false, true);
		}
	}

	//switching weapon
	if (m_wpn_switch && m_anim_timer.get_elapsed_msec() >= m_wpn_timer && !m_wpn_call) {
		m_wpn_call = true;
		if (!m_wpn_hidden_1 && !m_wpn_hidden_2) {
			m_wpn_holster = true;

			///m_weapon->action				( 4 );
			m_weapon->action				( 0 );
			update_animations(false, false, false, true, false);
		}
		else {
			switch_weapon();
		}
	}

	//if shoot, wait till anim ends
	if (m_start_shoot_timer && m_anim_timer.get_elapsed_msec() >= m_shoot_anim_time) {
		m_start_shoot_timer = false;
		m_wpn_shoot = false;
		m_weapon->action				( 0 );
	}

//	if (!m_wpn_switch) {
//		//update timer consistenly until we are switching weapon
//		m_wpn_timer = m_anim_timer.get_elapsed_msec() + 100;
//	}

	render::scene_ptr scene			= m_game_world.get_render_scene();
	render::game::renderer& r		= m_game_world.renderer();

	
	float4x4 const m				= create_rotation(float3(0.0f, math::pi, 0.0f)) * m_character_transform;
	
	r.scene().update_model			( scene, m_character_model->m_render_model, m * create_translation(float3(0.f, 0.04f, 0.f)));

	u32 const non_root_bones_count	= m_character_model->m_skeleton->get_non_root_bones_count( );
	float4x4* const matrices		= static_cast<float4x4*>( ALLOCA(non_root_bones_count*sizeof(float4x4)) );
	m_animation_player->compute_bones_matrices( *m_character_model->m_skeleton, matrices, matrices + non_root_bones_count );

	r.scene().update_skeleton		( m_character_model->m_render_model, matrices, non_root_bones_count );

	if (m_weapon && !m_weapon->m_hidden) {
		// update weapon
		float4x4 weapon_matrix;
		calculate_weapon_matrix(matrices, weapon_matrix);
		m_weapon->set_transform(weapon_matrix);
		m_weapon->tick(m_animation_player);
	}

	//calculate_head_matrix		( matrices, m_character_head_transform );

	calculate_camera_matrix		( matrices, m_character_camera_transform );

	// im bad with math :(
#if 0
	if (m_actor_input_controller->is_doing_movement() && !g_thirdperson_value)
	{
		float const frame_time_sec = m_actor_input_controller->last_frame_time_delta() / 1000.0f;
		float const bobbing_factor = frame_time_sec * 2.f;

		static float s_bobbing_time = 0.f;

		if (m_actor_input_controller->on_frame_sprint())
			s_bobbing_time += bobbing_factor * 1.2f;
		else
			s_bobbing_time += bobbing_factor;
		

		float3 bobbing = float3(0.0, 0.0, cos(sin(s_bobbing_time)));
		float4x4 bobbing_transform = create_translation(bobbing);
		m_character_head_transform = bobbing_transform * m_character_head_transform;
	}
#endif

#if 0
	if (g_thirdperson_value)
	{
		const float radius			= 2.f;
		const float hover_radius	= 0.2f;
		float3 character_position	= m_actor_physics_controller->get_transform().c.xyz();
		float3 thirdperson_offset	= float3(radius, radius, radius);
		float3 character_rotation	= m_character_head_transform.get_angles_xyz();
		
		float3 camera_position		= character_position + thirdperson_offset + float3(character_rotation.y + hover_radius, 0.f, 0.f);
		float4x4 camera_transform	= create_translation(camera_position);
		m_character_head_transform	= camera_transform;
	}
#endif

#if 0
	// other stuff (test, temp etc)
	{
		float3 ray_from		= m_character_camera_transform.c.xyz(); //m_character_head_transform.c.xyz();
		float3 ray_dir		= m_character_camera_transform.k.xyz(); //m_character_head_transform.k.xyz();
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
#endif

	// pressed fire action
	if(m_actor_input_controller && m_actor_input_controller->on_frame_fire() && !m_wpn_switch && !m_wpn_reload  && !m_wpn_hidden_1 && !m_wpn_hidden_2)
	{
		render::debug::renderer& d	= r.debug();
		float3 ray_from		= m_character_camera_transform.c.xyz(); //m_character_head_transform.c.xyz();
		float3 ray_dir		= m_character_camera_transform.k.xyz(); //m_character_head_transform.k.xyz();
		float ray_length	= 100.0f; // weapon config???
		
		m_weapon->action				( 1 );//shoot

		m_wpn_shoot = true; //play shoot anim

		physics::closest_ray_result result = m_game_world.get_physics_world()->ray_test( ray_from, ray_dir, ray_length );
		if (result.m_object) // we are pick something
		{
			d.draw_aabb( scene, result.m_hit_point_world, float3(0.01f,0.01f,0.01f), math::color(0,255,0,255));
			
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

void actor::calculate_camera_matrix(float4x4* const matrices, float4x4& result) const
{
	float4x4 character_render_transform = create_rotation(float3(0.0f, math::pi, 0.0f)) * m_character_transform * create_translation(float3(0.f, 0.04f, 0.f));
	result = (create_rotation(float3(0, math::pi, 0)) *
		matrices[m_camera_bone_idx] *
		character_render_transform);

	//result.c.xyz() += result.j.xyz() * 0.1f;
}

void actor::calculate_head_matrix( float4x4* const matrices, float4x4& result ) const
{
	float4x4 character_render_transform		= create_rotation(float3(0.0f, math::pi, 0.0f)) * m_character_transform * create_translation(float3(0.f, 0.04f, 0.f));
	result							= ( create_rotation(float3(0,0,math::pi_d2)) * 
											matrices[m_head_bone_idx] * 
											character_render_transform );

	result.c.xyz()			+= result.j.xyz()*0.1f;
}

void actor::calculate_weapon_matrix( float4x4* const matrices, float4x4& result  ) const
{
	float4x4 character_render_transform	= create_rotation(float3(0.0f, math::pi, 0.0f)) * m_character_transform * create_translation(float3(0.f, 0.04f, 0.f));
	result								= matrices[m_weapon_bone_idx] * character_render_transform;
}

}
