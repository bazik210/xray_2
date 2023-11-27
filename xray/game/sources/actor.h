////////////////////////////////////////////////////////////////////////////
//	Created		: 28.07.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef ACTOR_H_INCLUDED
#define ACTOR_H_INCLUDED

//#include "game_camera.h"
#include <xray/render/facade/model.h>
#include <xray/animation/animation_player.h>
#include <xray/animation/instant_interpolator.h>
#include "object_volumetric_sound.h"
#include <xray/render/facade/scene_renderer.h>
#include "weapon.h"

namespace xray{
namespace physics{
	class bt_character_controller;
	class bt_rigid_body;
}

namespace collision{ class geometry_instance; } 
}

namespace stalker2{

static bool g_noclip_enabled = false;

class game_world;
class actor_input_controller;

class actor : private boost::noncopyable //public game_camera

{
public:
					actor				( game_world& w );
					~actor				( );
	void			query_resources		( );

	void			tick				( );
	void			set_input_source	( actor_input_controller* s );

	collision::geometry_instance&	get_caracter_capsule		( );
	float4x4 const&					character_select_transform	( ) const;
	float4x4 const&					character_head_transform	( ) const		{ return m_character_head_transform; }
	float4x4 const&					character_camera_transform	( ) const { return m_character_camera_transform; }

	void			activate			( math::float4x4 const& initial_matrix );

	bool			m_stop_query;

	float			get_heath()	const { return m_health; }
	bool			get_particle_timer() const { return m_start_particle_timer; }
	void			clear_weapon_fx();

	void			fire_particle_load			( );
	void			particle_fire_attach		(resources::queries_result& data);
	
	bool			m_noclip;

	void			disable_noclip				( );

private:
	void			on_resources_ready			( resources::queries_result& data );
	void			on_load_animations			( resources::queries_result& data );
	void			add_models_to_scene			( );
	void			remove_models_from_scene	( );
	void			process_walk				( );
	void			calculate_walk_frames		( float& additive_walk_anim_time );

	void			process_input_events		( );
	void			update_animations			( bool m_reload = false, bool m_shoot = false, bool m_draw = false, bool m_holster = false, bool m_idle = false, bool m_crouch  = false);
	void			calculate_head_matrix		( float4x4* const matrices, float4x4& result ) const;
	void			calculate_weapon_matrix		( float4x4* const matrices, float4x4& result ) const;
	void			calculate_camera_matrix		(float4x4* const matrices, float4x4& result) const;
	void			switch_weapon				( );
	void			on_weapon_loaded			(resources::queries_result& data);
	void			query_new_weapon			(pstr weapon_type, pstr new_anim);
	void			disable_crouch				( );
	void			draw_debug_aabb				(render::game::renderer& r, render::scene_ptr& scene);

	static animation::mixing::animation_lexeme get_reload_lexeme(mutable_buffer& buffer, animation::skeleton_animation_ptr animation, float additive);

	float4x4							m_character_transform;
	float4x4							m_character_head_transform;
	float4x4							m_character_camera_transform;
	float								m_look_pitch;
	float4x4							m_weapon_matrix;
	float4x4							m_particle_matrix;

	render::skeleton_model_ptr			m_character_model;

	animation::animation_player*		m_animation_player;
	animation::animation_player*		m_animation_player2; //hack :(
	animation::instant_interpolator		m_interpolator;
	animation::skeleton_animation_ptr	m_idle_stand_animation;
	animation::skeleton_animation_ptr	m_look_animation_add; //additive
	animation::skeleton_animation_ptr	m_reload_animation;
	animation::skeleton_animation_ptr	m_shoot_animation;
	animation::skeleton_animation_ptr	m_draw_animation;
	animation::skeleton_animation_ptr	m_holster_animation;
	animation::skeleton_animation_ptr	m_idle_stand_01_animation;
	animation::skeleton_animation_ptr	m_crouch_animation;
	animation::skeleton_animation_ptr	current_additive_animation;
	animation::skeleton_animation_ptr	m_forward_animation;
	animation::skeleton_animation_ptr	m_right_animation;
	animation::skeleton_animation_ptr	m_left_animation;
	animation::skeleton_animation_ptr	current_walk_animation;
	
	xray::particle::particle_system_instance_ptr	m_particle_system_instance_ptr;

	timing::timer						m_anim_timer;
	animation::bone_index_type			m_head_bone_idx;
	animation::bone_index_type			m_camera_bone_idx;
	animation::bone_index_type			m_weapon_bone_idx;
	bool								m_tmp_is_active;
	bool								m_start_reload_timer;
	bool								m_start_shoot_timer;
	bool								m_start_draw_timer;
	bool								m_start_holster_timer;
	bool								m_start_particle_timer;
	bool								m_switch_snd_time_delay;
	bool								m_wpn_reload;
	bool								m_wpn_draw;
	bool								m_wpn_holster;
	bool								m_wpn_switch;
	bool								m_wpn_call;
	bool								m_wpn_hidden_1;
	bool								m_wpn_hidden_2;
	u32									m_wpn_timer;
	u32									m_reload_anim_time;
	u32									m_shoot_anim_time;
	u32									m_draw_anim_time;
	u32									m_holster_anim_time;
	u32									m_switch_snd_time;
	u32									m_particle_time;
	bool								m_wpn_shoot;
	bool								m_walk;	
	bool								m_walk_bcwd;
	float								m_walk_speed;
	int									m_frames;
	
	object_volumetric_sound*			m_snd;
	xray::physics::bt_character_controller*	m_actor_physics_controller;

	weapon_ptr							m_weapon;
	pstr								m_new_weapon;
	pstr								m_new_anim;
	u32									m_bone_count;

	actor_input_controller*				m_actor_input_controller;
	game_world&							m_game_world;

	float								m_health;
	float4								m_muzzle_point;
	math::float4x4						m_locator_offset;

	xray::fixed_string512				m_temp;
	xray::fixed_string512				m_plr_dir;
	xray::fixed_string512				m_weapon_dir;
	xray::fixed_string512				m_hud_global_test = "";
	xray::fixed_string512				m_weapon_c_sav = "";
	xray::fixed_string512				m_wpn_cmd = "";
}; // class actor

} // namespace stalker2

#endif // #ifndef ACTOR_H_INCLUDED
