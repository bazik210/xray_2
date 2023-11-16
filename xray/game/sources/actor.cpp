////////////////////////////////////////////////////////////////////////////
//	Created		: 28.07.2011
//	Author		: Andrew Kolomiets
//	Editors		: loxotron, ugo_zapad, Dieg
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "actor.h"
#include <xray/animation/instant_interpolator.h>
#include <xray/animation/mixing_animation_lexeme.h>
#include <xray/animation/mixing_math.h>
#include <xray/animation/mixing.h>
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
#include <xray/render/facade/material_effects_instance_cook_data.h>
#include "actor_input_controller.h"
#include "weapon.h"
#include "key_binder.h"
#include <xray/console_command.h>

using xray::animation::mixing::playing_type_enum;

namespace stalker2{

console_commands::cc_bool g_noclip("noclip", g_noclip_enabled, false, console_commands::command_type_user_specific);

static xray::command_line::key	s_hud		 ("hud", "", "", "player model");

static string512 weapon_c = "assault_rifles/ak74m";

static console_commands::cc_string weapon_cc ("give_me_weapon", weapon_c, 256, false, xray::console_commands::command_type_user_specific );

static xray::command_line::key	weapon_cl		 ("weapon", "", "", "on load");

static bool g_thirdperson_value = false;
console_commands::cc_bool g_thirdperson("thirdperson", g_thirdperson_value, false, console_commands::command_type_user_specific);

actor::actor( game_world& w )
:m_character_transform( float4x4().identity() ),
m_look_pitch		( 0.0f ),
m_actor_input_controller( NULL ),
m_animation_player	( NULL ),
m_animation_player2 ( NULL ),
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
m_start_particle_timer ( 0 ),
m_start_holster_timer ( 0 ),
m_switch_snd_time_delay ( 0 ),
m_switch_snd_time		( 0 ),
m_wpn_switch		( false ),
m_wpn_call			( false ),
m_noclip			( false ),
m_camera_bone_idx	( NULL ),
m_muzzle_point		(float4(0.0f, 0.03f, -0.8f, 1.0f)),
m_new_weapon		("assault_rifles/ak74m"),
m_new_anim			("resources/animations/single/weapons/assault_rifles/ak74m/1st_person/danger/player/reload")
{
	m_animation_player			= NEW(animation::animation_player)( );
	m_animation_player->set_no_delete();// ??

	m_animation_player2			= NEW(animation::animation_player)( );
	m_animation_player2->set_no_delete();

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

		DELETE(m_animation_player2);
		m_animation_player2 = NULL;
}

void actor::set_input_source( actor_input_controller* s )
{
	m_actor_input_controller = s;
}

void actor::query_resources( )
{
	//-hud="actor/neutral_01/neutral_01" -weapon="ak_74" //example keys 
	//-hud="neutral\neutral_01_novice\neutral_01_novice" -weapon="assault_rifles/ak74m" //example keys
	//-hud="dolg/dolg_exo" -weapon="ak_74" //example keys 

	s_hud.is_set_as_string( &m_temp );

	m_weapon_c_sav = weapon_c;
	m_weapon_dir = weapon_c;
	if (m_temp != "")
	{
		m_plr_dir	= "character/human/";	//first part of
		m_plr_dir.append(m_temp);			//it's like plr_dir+=temp
		m_plr_dir.replace("\\","/");		//fixing incorrect separators (esli est \_(._.)_/)
	
	}
	else m_plr_dir = "character/human/actor/neutral_03/neutral_03_actor_full"; //the default hud model
	m_hud_global_test = m_plr_dir;
	if (m_weapon_dir == "") m_weapon_dir = "assault_rifles/ak74m";//the default weapon model

	m_wpn_cmd = weapon_c;

	weapon_cl.is_set_as_string(&m_wpn_cmd);

	resources::request r[] ={
		{ m_plr_dir.c_str(),	resources::skeleton_model_instance_class },
		{ m_weapon_dir.c_str(),	resources::weapon_class },
	};

	m_new_weapon = m_weapon_dir.c_str(); 

	if (m_wpn_cmd != "") {
		m_new_weapon = m_wpn_cmd.c_str();
	}

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

	m_head_bone_idx			= m_character_model->m_skeleton->get_bone_index("Head")-1;

	if(m_character_model->m_skeleton->bone_exist("Camera_Root"))
		m_camera_bone_idx		= m_character_model->m_skeleton->get_bone_index("Camera_Root")-1;

	m_weapon_bone_idx		= m_character_model->m_skeleton->get_bone_index("Weapon")-1;

	m_weapon				= static_cast_resource_ptr<weapon_ptr>(data[1].get_unmanaged_resource());
	m_weapon->m_game_world	= &m_game_world;


	m_bone_count = m_character_model->m_skeleton->get_bones_count();

	if (m_bone_count > 65) {
		resources::request r[] = {
		{ "resources/animations/single/human/actor/locomotion/stand/on_site_idle",	resources::animation_class },
		{ "resources/animations/single/human/actor/locomotion/stand/unarmed_on_site_idle",  resources::animation_class },
		{ "resources/animations/single/human/actor/locomotion/stand/on_site_add",	resources::animation_class },
		{ "resources/animations/single/weapons/assault_rifles/ak74m/1st_person/danger/player/reload_empty", resources::animation_class },
		{ "resources/animations/single/weapons/assault_rifles/ak74m/1st_person/danger/player/fire_1",  resources::animation_class },
		{ "resources/animations/single/weapons/assault_rifles/ak74m/1st_person/danger/player/draw",  resources::animation_class },
		{ "resources/animations/single/weapons/assault_rifles/ak74m/1st_person/danger/player/holster",  resources::animation_class },
		{ "resources/animations/single/human/actor/locomotion/crouch/on_site_idle",  resources::animation_class }
		};

		resources::query_resources(
			r,
			boost::bind(&actor::on_load_animations, this, _1),
			g_allocator
		);
	}
	else {
		resources::request r[] = {
		{ "resources/animations/single/human/hud/stand_idle",  resources::animation_class },
		{ "resources/animations/single/human/hud/stand_idle",  resources::animation_class },
		{ "resources/animations/single/human/hud/stand_add",   resources::animation_class },
		{ "resources/animations/single/human/hud/reload",	   resources::animation_class },
		{ "resources/animations/single/human/hud/stand_idle",  resources::animation_class },
		{ "resources/animations/single/human/hud/stand_idle",  resources::animation_class },
		{ "resources/animations/single/human/hud/stand_idle",  resources::animation_class },
		{ "resources/animations/single/human/hud/stand_idle",  resources::animation_class }
		};

		resources::query_resources(
			r,
			boost::bind(&actor::on_load_animations, this, _1),
			g_allocator
		);
	}
}

void actor::on_load_animations(  resources::queries_result& data  ) 
{
	m_idle_stand_animation	= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[0].get_managed_resource());
	m_idle_stand_01_animation	= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[1].get_managed_resource());
	m_look_animation_add	= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[2].get_managed_resource());
	m_reload_animation		= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[3].get_managed_resource());
	m_shoot_animation		= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[4].get_managed_resource());
	m_draw_animation		= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[5].get_managed_resource());
	m_holster_animation		= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[6].get_managed_resource());
	m_crouch_animation		= static_cast_resource_ptr<animation::skeleton_animation_ptr>(data[7].get_managed_resource());

	//some query system bug;
	m_stop_query = true;

	m_game_world.tmp_actor_ready( this );

	fire_particle_load();
}

void actor::clear_weapon_fx()
{
	m_game_world.renderer().scene().remove_particle_system_instance(m_game_world.get_render_scene(), m_particle_system_instance_ptr);
}

void actor::fire_particle_load() {

	xray::render::material_effects_instance_cook_data* cook_data = 
		NEW(xray::render::material_effects_instance_cook_data)(xray::render::decal_vertex_input_type, NULL, false);
	
	resources::user_data_variant		user_data;
	user_data.set					( cook_data );

	xray::render::material_effects_instance_cook_data* cook_data2 = 
		NEW(xray::render::material_effects_instance_cook_data)(xray::render::decal_vertex_input_type, NULL, false);

	resources::user_data_variant		user_data2;
	user_data2.set					( cook_data2 );

	xray::render::material_effects_instance_cook_data* cook_data3 = 
		NEW(xray::render::material_effects_instance_cook_data)(xray::render::decal_vertex_input_type, NULL, false);

	resources::user_data_variant		user_data3;
	user_data3.set					( cook_data3 );

	xray::render::material_effects_instance_cook_data* cook_data4 = 
		NEW(xray::render::material_effects_instance_cook_data)(xray::render::decal_vertex_input_type, NULL, false);

	resources::user_data_variant		user_data4;
	user_data4.set					( cook_data4 );

	resources::request r[] = {
		{ "fx/pfx/pfx_flame_01",	resources::material_effects_instance_class },
		{ "fx/pfx/glow_fire",	resources::material_effects_instance_class },
		{ "fx/pfx/smoke_03",	resources::material_effects_instance_class },
		{ "fx/pfx/distort_14_panner",	resources::material_effects_instance_class },
//		{ "campfire_middle",	resources::particle_system_instance_class },
		{ "fx_weapon",	resources::particle_system_instance_class }
	};

	resources::user_data_variant udv;
	particle::world* p = &m_game_world.renderer().scene().particle_world(m_game_world.get_render_scene());
	udv.set(p);

//	resources::user_data_variant const* user_data[] = { &udv };
	resources::user_data_variant const* ud[] = { &user_data, &user_data2, &user_data3, &user_data4, &udv };

	resources::query_resources(
		r,
		boost::bind( &actor::particle_fire_attach, this, _1 ),
		g_allocator,
		ud
	);
}

void actor::particle_fire_attach( resources::queries_result& data ) 
{
	R_ASSERT(data.is_successful());
//	LOG_INFO("SUCCESS!");

	m_particle_system_instance_ptr = static_cast_resource_ptr<xray::particle::particle_system_instance_ptr>( data[4].get_unmanaged_resource());

}

void actor::add_models_to_scene( )
{
	render::scene_ptr scene		= m_game_world.get_render_scene();
	render::game::renderer& r	= m_game_world.renderer();

	r.scene().add_model			( scene, m_character_model->m_render_model, m_character_transform );
	switch_weapon();

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
	m_animation_player2->set_object_transform	( m_character_transform );
	add_models_to_scene				( );
	m_tmp_is_active					= true;
}

collision::geometry_instance& actor::get_caracter_capsule( )
{
	return m_actor_physics_controller->get_capsule( );
}

float4x4 const& actor::character_select_transform() const
{
	if (m_camera_bone_idx) {
		return m_character_camera_transform;
	}
	else {
		return m_character_head_transform;
	}
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

		float move_delta_fw = 0;
		float move_delta_right = 0;

		if (!g_noclip_enabled) {

			move_delta_fw = frame_time_sec * 1.66f * 4.f;
			move_delta_right = frame_time_sec * 0.83f * 4.f;
			if (m_actor_input_controller->on_frame_sprint() && !m_actor_input_controller->m_crouch)
			{
				move_delta_fw = frame_time_sec * 1.66f * 10.f;
				move_delta_right = frame_time_sec * 0.83f * 10.f;

				// reset if unpressed key
				if (!m_actor_input_controller->is_doing_movement())
					m_actor_input_controller->m_sprint_toggle = false;
			}
			if (m_actor_input_controller->on_frame_crouch()) {
				move_delta_fw = frame_time_sec * 1.66f * 2.f;
				move_delta_right = frame_time_sec * 0.83f * 2.f;
			}
		}
		else {
			move_delta_fw = frame_time_sec * 1.66f * 15.f;
			move_delta_right = frame_time_sec * 0.83f * 15.f;
		}

		//loxotron: we should update particle position in time somehow, cause it's not attached to anything, bad way :(
		if (m_actor_input_controller->onframe_move_fwd() && m_actor_input_controller->m_frame_events.action_present(kFWD)) {
				m_muzzle_point = float4(0.0f, 0.03f, -1.1f, 1.0f);
		}
		else if (m_actor_input_controller->onframe_move_right())
		{
			if (m_actor_input_controller->m_frame_events.action_present(kR_STRAFE)) {
				m_muzzle_point = float4(-0.09f, 0.03f, -0.8f, 1.0f);
			}
			else if (m_actor_input_controller->m_frame_events.action_present(kL_STRAFE))
			{
				m_muzzle_point = float4(0.09f, 0.03f, -0.8f, 1.0f);
			}
		}
		else {
			m_muzzle_point = float4(0.0f, 0.03f, -0.8f, 1.0f);
		}

		float3 walk_direction			= m_character_transform.k.xyz() * m_actor_input_controller->onframe_move_fwd() * move_delta_fw;
		walk_direction					+= m_character_transform.i.xyz() * m_actor_input_controller->onframe_move_right() * move_delta_right;

		
		walk_direction += m_character_transform.j.xyz() * m_actor_input_controller->onframe_move_up() * move_delta_fw;

		if (m_actor_input_controller->m_frame_events.m_onframe_mouse_move && g_noclip_enabled) {
			walk_direction = m_character_camera_transform.k.xyz() * m_actor_input_controller->m_frame_events.m_onframe_mouse_move * move_delta_fw;
		}

		m_actor_physics_controller->set_walk_direction( walk_direction  );
	}

	if (g_noclip_enabled && !m_noclip) {
		m_noclip = true;
		m_actor_physics_controller->set_noclip();
	}
	else if (!g_noclip_enabled && m_noclip) {
		disable_noclip();
	}

	if (m_actor_input_controller->onframe_jump() && !g_noclip_enabled) {
		m_actor_physics_controller->jump();
	}

//	pressed crouch
//	if (m_actor_input_controller && m_actor_input_controller->m_crouch && !m_wpn_reload && !m_wpn_switch && !m_wpn_holster && !m_wpn_draw && !m_wpn_hidden_1 && !m_wpn_hidden_2) 
//	{
//		m_actor_input_controller->m_crouch = !m_actor_input_controller->m_crouch;
//		//update_animations(false, false, false, false, false, true);
//		if (!m_crouch) {
//			m_crouch = true;
//			float4x4 character_position = m_actor_physics_controller->get_transform();
//			cr_y = character_position.c.y -= 0.5;
////////////////////////////////////////////////////////////////
//			m_actor_physics_controller->set_capsule_scaling(1, 0.5, 1, true);
//		}
//		else {
//			m_crouch = false;
//			//character_position.c.y += 0.5;
//			//m_actor_physics_controller->set_transform(character_position);
////////////////////////////////////////////////////////////////
//			disable_crouch();
//		}
//	}

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

animation::mixing::animation_lexeme actor::get_reload_lexeme(mutable_buffer& buffer, animation::skeleton_animation_ptr animation, float additive)
{
	static animation::mixing::animation_lexeme	current_reload_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			"reload",
			animation
		).time_scale(1.f)
//		.start_animation_interval_time(additive)
		//.playing_type(playing_type_enum::play_cyclically)
	);
	return current_reload_lexeme;
}

void actor::update_animations( bool m_reload, bool m_shoot, bool m_draw, bool m_holster, bool m_idle, bool m_crouch )
{

	mutable_buffer buffer	( ALLOCA( animation::animation_player::stack_buffer_size*2 ), animation::animation_player::stack_buffer_size*2 );

	animation::skeleton_animation_ptr current_idle_animation		= m_idle_stand_animation;
	animation::skeleton_animation_ptr current_idle_01_animation		= m_idle_stand_01_animation;
	animation::skeleton_animation_ptr current_additive_animation	= m_look_animation_add;
	animation::skeleton_animation_ptr current_shoot_animation		= m_shoot_animation;
	animation::skeleton_animation_ptr current_draw_animation		= m_draw_animation;
	animation::skeleton_animation_ptr current_holster_animation		= m_holster_animation;
	animation::skeleton_animation_ptr current_crouch_animation		= m_crouch_animation;
	animation::skeleton_animation_ptr current_reload_animation		= m_reload_animation;

	// calculate additive animation coefficient, based on pitch
	float k								= 1.0f - (m_look_pitch+1.0f)/2.0f; // normalized to 0..1.0f
	float additive_current_anim_time	= animation::cubic_spline_skeleton_animation_pinned( current_additive_animation ).c_ptr()->length_in_frames() / animation::default_fps * k;
	float additive_crouch_anim_time	= animation::cubic_spline_skeleton_animation_pinned( current_additive_animation ).c_ptr()->length_in_frames() / animation::default_fps;

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

	animation::mixing::animation_lexeme	current_crouch_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer,
			"crouch",
			current_crouch_animation
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

	animation::mixing::animation_lexeme	current_additive_crouch_lexeme(
		animation::mixing::animation_lexeme_parameters(
			buffer, 
			"additive_crouch",
			current_crouch_animation
		)
		.time_scale( 1.f )
		.start_animation_interval_time( additive_crouch_anim_time )
		.override_existing_animation( true )
		.additivity_priority( 1 )
	);

		animation::mixing::animation_lexeme	current_reload_lexeme(
			animation::mixing::animation_lexeme_parameters(
				buffer,
				"reload",
				m_reload_animation
			).time_scale(1.f)
		);

		animation::mixing::animation_lexeme	additive_reload_lexeme(
			animation::mixing::animation_lexeme_parameters(
				buffer,
				"additive",
				current_additive_animation
			)
			.time_scale(0.f)
			.start_animation_interval_time(additive_current_anim_time)
			.override_existing_animation(true)
			.additivity_priority(2)
		);

	u32 current_time				= m_anim_timer.get_elapsed_msec();	

	animation::mixing::animation_lexeme weapon_target = m_weapon->select_animation( buffer );

	m_weapon->select_animation(buffer);

	if (!m_reload && !m_shoot && !m_draw && !m_holster && !m_idle && !m_crouch) {
		m_animation_player->set_target_and_tick(
			current_idle_lexeme
			+ current_additive_lexeme
			+ weapon_target
			, current_time);
		m_animation_player2->set_target_and_tick(
			current_idle_lexeme
			+ current_additive_lexeme
			+ weapon_target
			, current_time);
	}
	else {
		if (m_reload) {
			if (!m_start_reload_timer) {
				m_start_reload_timer = true;
				m_snd = NEW(object_volumetric_sound)(m_game_world);
				m_snd->load_custom("reload", m_character_transform, false);
				m_reload_anim_time = current_time + (current_reload_lexeme.animation_intervals_begin()->length() * 1000);
				m_animation_player->set_target(
					current_reload_lexeme
					+ weapon_target
					, current_time);
			}
			m_animation_player->tick(current_time);
			if (m_bone_count > 65) {
				m_animation_player2->set_target_and_tick(
					current_reload_lexeme
					+ additive_reload_lexeme
					, current_time);
			}
		}
		else if (m_shoot) {
			if (!m_start_shoot_timer) {
				m_start_shoot_timer = true;
				m_snd = NEW(object_volumetric_sound)(m_game_world);
				m_snd->load_custom("shoot",  m_character_transform, false);
				m_shoot_anim_time = current_time + (current_shoot_lexeme.animation_intervals_begin()->length() * 1000);
			m_animation_player->set_target(
				current_shoot_lexeme
				+ current_additive_lexeme
				+ weapon_target
				, current_time);

			m_game_world.renderer().scene().play_particle_system( m_game_world.get_render_scene(), m_particle_system_instance_ptr, m_particle_matrix );

			m_particle_time = current_time + (current_shoot_lexeme.animation_intervals_begin()->length() * 1000) / 1.1;
			m_start_particle_timer = true;
			}
			m_animation_player->tick(current_time);
			m_animation_player2->set_target_and_tick(
				current_shoot_lexeme
				+ current_additive_lexeme
				+ weapon_target
				, current_time);
		}
		else if (m_draw) {
			if (!m_start_draw_timer) {
				m_start_draw_timer = true;
				m_snd = NEW(object_volumetric_sound)(m_game_world);
				m_snd->load_custom("draw", m_character_transform, false);
				m_draw_anim_time = current_time + (current_draw_lexeme.animation_intervals_begin()->length() * 1000);
			}
			m_animation_player->set_target_and_tick(
				current_draw_lexeme
				+ current_additive_lexeme
				+ weapon_target
				, current_time);
			m_animation_player2->set_target_and_tick(
				current_draw_lexeme
				+ current_additive_lexeme
				+ weapon_target
				, current_time);
		}
		else if (m_holster) {
			if (!m_start_holster_timer) {
				m_start_holster_timer = true;
				m_snd = NEW(object_volumetric_sound)(m_game_world);
				m_snd->load_custom("holster",  m_character_transform, false);
				m_holster_anim_time = current_time + (current_holster_lexeme.animation_intervals_begin()->length() * 1000);
			}
			m_animation_player->set_target_and_tick(
				current_holster_lexeme
				+ current_additive_lexeme
				+ weapon_target
				, current_time);
			m_animation_player2->set_target_and_tick(
				current_holster_lexeme
				+ current_additive_lexeme
				+ weapon_target
				, current_time);
		}
		else if (m_crouch) {
			m_animation_player->set_target_and_tick(
				current_crouch_lexeme
				+ current_additive_lexeme
				+ weapon_target
				, current_time);
			m_animation_player2->set_target_and_tick(
				current_crouch_lexeme
				+ current_additive_lexeme
				+ weapon_target
				, current_time);
		}
		else if (m_idle) {
			m_animation_player->set_target_and_tick(
				current_idle_no_wpn_lexeme
				+ current_additive_lexeme
				, current_time);
			m_animation_player2->set_target_and_tick(
				current_idle_no_wpn_lexeme
				+ current_additive_lexeme
				, current_time);
		}
	}
}

//not animated temp physics crouch
void actor::disable_crouch()
{
	m_actor_physics_controller->set_capsule_scaling(1, 1, 1, false);
	float4x4 pos = m_actor_physics_controller->get_transform();
	m_actor_physics_controller->deactivate();
	m_actor_physics_controller->clear();
	m_actor_physics_controller->initialize();
	m_actor_physics_controller->activate(pos);
}

void actor::tick()
{
	if (!m_tmp_is_active)
		return;

	// from previous physic step
	if (!m_actor_physics_controller)
		return;

	//updating particle position
	if (m_start_particle_timer)
		m_game_world.renderer().scene().update_particle_system_instance( m_game_world.get_render_scene(), m_particle_system_instance_ptr, m_particle_matrix );

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

	if (!m_actor_input_controller->on_frame_crouch() && m_actor_input_controller->m_crouch && !g_noclip_enabled)
	{
		m_actor_input_controller->m_crouch = false;

		disable_crouch();
	}
	else if (m_actor_input_controller->on_frame_crouch() && !m_actor_input_controller->m_crouch && !g_noclip_enabled)
	{
		m_actor_input_controller->m_crouch = true;

		m_actor_physics_controller->set_capsule_scaling(1, 0.5, 1, true);
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

	//if (!m_actor_input_controller->on_frame_crouch()) {
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
//	 }
//	 else {
//		 m_actor_physics_controller->set_capsule_scaling(1, 1, 1);
//		 update_animations(false, false, false, false, false, true);
//	 }

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

	if (m_weapon_c_sav != weapon_c)
	{
		m_weapon_c_sav = weapon_c;
		m_new_weapon = m_weapon_c_sav.c_str();
		switch_weapon();
	}

	//if shoot, wait till anim ends
	if (m_start_shoot_timer && m_anim_timer.get_elapsed_msec() >= m_shoot_anim_time) {
		m_start_shoot_timer = false;
		m_wpn_shoot = false;
		m_weapon->action				( 0 );
	}
	if (m_start_particle_timer && m_anim_timer.get_elapsed_msec() >= m_particle_time) {
		m_start_particle_timer = false;
		clear_weapon_fx();
	}


	render::scene_ptr scene			= m_game_world.get_render_scene();
	render::game::renderer& r		= m_game_world.renderer();

	
	float4x4 const m				= create_rotation(float3(0.0f, math::pi, 0.0f)) * m_character_transform;
	
	r.scene().update_model			( scene, m_character_model->m_render_model, m * create_translation(float3(0.f, 0.04f, 0.f)));

	u32 const non_root_bones_count	= m_character_model->m_skeleton->get_non_root_bones_count( );
	float4x4* const matrices		= static_cast<float4x4*>( ALLOCA(non_root_bones_count*sizeof(float4x4)) );
	m_animation_player->compute_bones_matrices( *m_character_model->m_skeleton, matrices, matrices + non_root_bones_count );
	m_animation_player2->compute_bones_matrices( *m_character_model->m_skeleton, matrices, matrices + non_root_bones_count );

	r.scene().update_skeleton		( m_character_model->m_render_model, matrices, non_root_bones_count );

	if (m_weapon && !m_weapon->m_hidden) {
		// update weapon
		calculate_weapon_matrix(matrices, m_weapon_matrix);
		m_weapon->set_transform(m_weapon_matrix);
		m_weapon->tick(m_animation_player);

		/*
		* Dieg:
		* Particle position magic starts here.
		* 1. Start by using local coordinates (so the point is using coordinates in relation to coordinates of our weapon)
		* 2. Play with numbers until particle appears where it has to in default position right after spawning inside the scene*
		* 3. Rotate our point around origin so that it always appears at the muzzle
		* 4. Convert coordinates to weapon space (just add local pos to weapon pos)
		*
		* *We should consider predefined muzzle_pos in weapon config file just like it was done in X-Ray 1.x so that we don't use magic numbers in code.
		* When dealing with weapon attachments such as changing barrel length or adding a silencer we could just create onAttach() and onDetach() functions
		* that would simply do m_muzzle_pos = m_muzzle_pos + val_from_config .
		*/
		// 1 + 2
		// X and Z values are minus because it seems that some unspeakable evil is happening during rotation (jk, I just didn't want to read the code).
		// float4 muzzle_point = float4(-0.02f, 0.0f, -1.1f /*-0.7f*/, 1.0f);
		// 3
		// It seems that we are using row order so vec4 * mat4x4 = vec4. Multiplying the point by rotation matrix returns coordinates after rotation.

		 float4 muzzle_point_rotated = m_muzzle_point * create_rotation(m_weapon_matrix.get_angles_xyz());
		// 4
		m_particle_matrix = m_weapon_matrix; //m_weapon->m_barrel_end->m_transform;

		//old ak (wpn_2) hasn't this type of locator, just notification
		//m_locator_offset = m_weapon->m_barrel_end->get_muzzle_flash_locator().m_offset;

		m_particle_matrix.c.x = m_particle_matrix.c.x + muzzle_point_rotated.x; //- m_locator_offset.c.x; //
		m_particle_matrix.c.y = m_particle_matrix.c.y + muzzle_point_rotated.y; //- m_locator_offset.c.y; //
		m_particle_matrix.c.z = m_particle_matrix.c.z + muzzle_point_rotated.z; //- m_locator_offset.c.z; //
	}


	if (m_camera_bone_idx) {
		calculate_camera_matrix(matrices, m_character_camera_transform);
	}
	else {
		calculate_head_matrix( matrices, m_character_head_transform );
	}

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
	if(!g_noclip_enabled && m_game_world.get_game().get_active_scene_view() && m_actor_input_controller && m_actor_input_controller->on_frame_fire() && !m_wpn_switch && !m_wpn_reload && !m_wpn_hidden_1 && !m_wpn_hidden_2)
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

	process_input_events();
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

	result.k.xyz() += result.k.xyz() / 1.6; //forward vector
	result.c.xyz() += result.j.xyz() * 0.1f;
}

void actor::calculate_weapon_matrix( float4x4* const matrices, float4x4& result  ) const
{
	float4x4 character_render_transform	= create_rotation(float3(0.0f, math::pi, 0.0f)) * m_character_transform * create_translation(float3(0.f, 0.04f, 0.f));
	result								= matrices[m_weapon_bone_idx] * character_render_transform;
}

void actor::disable_noclip()
{
	g_noclip_enabled = false;
	m_noclip = false;
	m_actor_physics_controller->set_noclip(false);
}

}