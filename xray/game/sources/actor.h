////////////////////////////////////////////////////////////////////////////
//	Created		: 28.07.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED

//#include "game_camera.h"
#include <xray/render/facade/model.h>
#include <xray/animation/animation_player.h>
#include <xray/animation/instant_interpolator.h>
#include "weapon.h"

namespace xray{
namespace physics{
	class bt_character_controller;
	class bt_rigid_body;
}

namespace collision{ class geometry_instance; } 
}

namespace stalker2{

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
	float4x4 const&					character_head_transform	( ) const		{ return m_character_head_transform; }

	void			activate			( math::float4x4 const& initial_matrix );

	bool			m_stop_query;


	float			get_heath()	const { return m_health; }

private:
	void			on_resources_ready			( resources::queries_result& data );
	void			add_models_to_scene			( );
	void			remove_models_from_scene	( );

	void			process_input_events		( );
	void			update_animations			( );
	void			calculate_head_matrix		( float4x4* const matrices, float4x4& result ) const;
	void			calculate_weapon_matrix		( float4x4* const matrices, float4x4& result ) const;

	float4x4							m_character_transform;
	float4x4							m_character_head_transform;
	float								m_look_pitch;

	render::skeleton_model_ptr			m_character_model;

	animation::animation_player*		m_animation_player;
	animation::instant_interpolator		m_interpolator;
	animation::skeleton_animation_ptr	m_idle_stand_animation;
	animation::skeleton_animation_ptr	m_look_animation_add; //additive
	animation::skeleton_animation_ptr	m_reload_animation;

	timing::timer						m_anim_timer;
	animation::bone_index_type			m_head_bone_idx;
	animation::bone_index_type			m_weapon_bone_idx;
	bool								m_tmp_is_active;

	xray::physics::bt_character_controller*	m_actor_physics_controller;

	weapon_ptr							m_weapon;

	actor_input_controller*				m_actor_input_controller;
	game_world&							m_game_world;

	float								m_health;
}; // class actor

} // namespace stalker2

#endif // #ifndef HUD_H_INCLUDED
