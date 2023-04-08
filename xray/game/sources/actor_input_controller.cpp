////////////////////////////////////////////////////////////////////////////
//	Created		: 23.09.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "actor_input_controller.h"
#include "game_scene.h"
#include "game.h"
#include "key_binder.h"
#include <xray/input/keyboard.h>
#include <xray/input/mouse.h>
#include <xray/input/world.h>

namespace stalker2
{

actor_input_controller::actor_input_controller( game_scene& w )
:super			( w ),
m_sprint_toggle ( false )
{
};


bool actor_input_controller::on_keyboard_action(input::world* input_world, 
												input::enum_keyboard key, 
												input::enum_keyboard_action action )
{
	XRAY_UNREFERENCED_PARAMETERS	( input_world );

	game_action_id game_action = stalker2::key_binder::get_binded_action( key );

	if ( game_action == kNOTBINDED )
		return false;
	
	if(action == xray::input::kb_key_hold)
		m_frame_events.m_game_actions.push_back	( game_action );
	else if ( action == xray::input::kb_key_down && game_action == kSPRINT_TOGGLE )
		m_sprint_toggle = !m_sprint_toggle;
	return false;
}

bool actor_input_controller::on_gamepad_action( input::world* input_world, 
											input::gamepad_button button, 
											input::enum_gamepad_action action )
{
	XRAY_UNREFERENCED_PARAMETERS	( input_world, button, action);
	return	false;
}

bool actor_input_controller::on_mouse_key_action( input::world* input_world, 
											input::mouse_button button, 
											input::enum_mouse_key_action action )
{
	XRAY_UNREFERENCED_PARAMETERS	( input_world );

	game_action_id game_action = stalker2::key_binder::get_binded_action( button );

	if ( game_action == kNOTBINDED )
		return false;

	if(action==xray::input::ms_key_down)
		m_frame_events.m_game_actions.push_back	( game_action );

	return false;
}

bool actor_input_controller::on_mouse_move( input::world* input_world, int x, int y, int z )
{
	XRAY_UNREFERENCED_PARAMETERS	( input_world );

	m_frame_events.m_mouse_move.x += x;
	m_frame_events.m_mouse_move.y += y;
	m_frame_events.m_mouse_move.z += z;
	return false;
}

bool actor_input_controller::on_frame_fire		( ) 
{ 
	return m_frame_events.action_present(kWPN_FIRE); 
}

bool frame_events::action_present( game_action_id game_action_name ) const
{
	return std::find(m_game_actions.begin(), m_game_actions.end(), game_action_name) != m_game_actions.end();
}

bool frame_events::empty( ) const
{
	return( m_game_actions.empty() && 
		math::is_zero(m_mouse_move.x) && 
		math::is_zero(m_mouse_move.y) && 
		math::is_zero(m_mouse_move.z) );
}

void frame_events::reset( )
{
	m_game_actions.clear	( );
	m_mouse_move.set		( 0.0f, 0.0f, 0.0f );
	m_onframe_move_fwd		= 0.0f;
	m_onframe_move_right	= 0.0f;
	m_onframe_turn_y		= 0.0f;
	m_onframe_turn_x		= 0.0f;
	m_onframe_jump			= false;
}

void actor_input_controller::on_before_processing( input::world* input_world )
{
	XRAY_UNREFERENCED_PARAMETERS	( input_world );
	m_frame_events.reset	( );
}

void actor_input_controller::on_after_processing( input::world* input_world )
{
	XRAY_UNREFERENCED_PARAMETERS			( input_world );

	u32 const time_ms						= m_timer.get_elapsed_msec();

	ASSERT									( time_ms >= m_frame_events.m_last_frame_time_ms );
	u32 const curr_delta					= (time_ms - m_frame_events.m_last_frame_time_ms);
	m_frame_events.m_last_frame_time_delta	= curr_delta*0.1f + m_frame_events.m_last_frame_time_delta*0.9f;

	m_frame_events.m_last_frame_time_ms		= time_ms;

	m_frame_events.m_onframe_move_fwd		= 0.f;
	m_frame_events.m_onframe_move_right		= 0.f;
	m_frame_events.m_onframe_turn_x			= 0.f;
	m_frame_events.m_onframe_turn_y			= 0.f;
	m_frame_events.m_onframe_jump			= false;

	if( m_frame_events.empty() )
		return;

	float move_forward_speed = 1.f;
	if ( m_frame_events.action_present(kACCEL) || m_sprint_toggle )
		move_forward_speed = 1.5f;

	if ( m_frame_events.action_present(kFWD) )
		m_frame_events.m_onframe_move_fwd		+= move_forward_speed;

	if ( m_frame_events.action_present(kBACK) )
		m_frame_events.m_onframe_move_fwd		-= 1.0f;

	if ( m_frame_events.action_present(kR_STRAFE) )
		m_frame_events.m_onframe_move_right		+= 1.0f;

	if ( m_frame_events.action_present(kL_STRAFE) )
		m_frame_events.m_onframe_move_right		-= 1.0f;

	if ( m_frame_events.action_present(kJUMP) )
		m_frame_events.m_onframe_jump		= true;

	if ( m_frame_events.action_present(kRIGHT) )
		m_frame_events.m_mouse_move.x			+= 1.0f;

	if ( m_frame_events.action_present(kLEFT) )
		m_frame_events.m_mouse_move.x			-= 1.0f;


	if ( m_frame_events.action_present(kUP) )
		m_frame_events.m_mouse_move.y			-= 1.0f;

	if ( m_frame_events.action_present(kDOWN) )
		m_frame_events.m_mouse_move.y			+= 1.0f;

	m_frame_events.m_onframe_turn_x		= math::deg2rad( m_frame_events.m_mouse_move.y );
	m_frame_events.m_onframe_turn_y		= math::deg2rad( m_frame_events.m_mouse_move.x ) * 0.75f;
	
}

void actor_input_controller::update_camera_matrix( frame_events const& frame_events, math::float4x4& camera_matrix )
{
	math::float2 const raw_angles	= float2( frame_events.m_onframe_turn_x, frame_events.m_onframe_turn_y );
	float const move_fwd			= frame_events.m_onframe_move_fwd;
	float const move_right			= frame_events.m_onframe_move_right;

	float const linear_factor		= 0.006f * frame_events.m_last_frame_time_delta;
	float const angle_factor		= 0.5f;

	float3 const angles_zxy			= camera_matrix.get_angles( math::rotation_zxy );

	float3 new_angles_zxy			= float3(	angle_factor*raw_angles.x + angles_zxy.x, 
		angle_factor*raw_angles.y + angles_zxy.y, 
		angles_zxy.z );

	float const x_angle_margin		= 0.001f; // let animation clamps vertical angles math::pi_d8/2.0f;
	new_angles_zxy.x				= math::clamp_r( new_angles_zxy.x, 
													-math::pi_d2+x_angle_margin, 
													math::pi_d2-x_angle_margin );

	float4x4 rotation				= math::create_rotation( new_angles_zxy, math::rotation_zxy );

	float3 const position			=	camera_matrix.c.xyz( ) +
										camera_matrix.i.xyz( ) * (linear_factor*move_right) +
										camera_matrix.k.xyz( ) * (linear_factor*move_fwd);

	float4x4 const translation		= math::create_translation( position );

	camera_matrix					= rotation * translation;
}


void actor_input_controller::on_focus( bool b_focus_enter )
{
	super::on_focus ( b_focus_enter );

	if(b_focus_enter)
	{
		m_game_scene.get_game().input_world().add_handler( *this );
		m_timer.start							( );
		m_frame_events.m_last_frame_time_ms		= 0;
		m_frame_events.m_last_frame_time_delta	= 0;
	}else
	{
		m_game_scene.get_game().input_world().remove_handler( *this );
	}
}

} //namespace stalker2
