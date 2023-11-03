////////////////////////////////////////////////////////////////////////////
//	Created		: 23.09.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef ACTOR_INPUT_CONTROLLER_H_INCLUDED
#define ACTOR_INPUT_CONTROLLER_H_INCLUDED

#include <xray/input/handler.h>
#include "game_camera.h"

namespace stalker2{

class game_scene;
enum game_action_id;

struct frame_events
{
	frame_events():m_last_frame_time_ms(0),m_last_frame_time_delta(0.0f) {reset();}
	
	bool		action_present		( game_action_id game_action_name ) const;
	bool		empty				( ) const;
	void		reset				( );

	u32						m_last_frame_time_ms;
	float					m_last_frame_time_delta;
	vector<int>				m_game_actions;
	
	float3					m_mouse_move;

	float					m_onframe_move_fwd;
	float					m_onframe_move_right;
	float					m_onframe_turn_y;
	float					m_onframe_turn_x;
	bool					m_onframe_jump;
};

class actor_input_controller:	public xray::input::handler, 
								public game_camera
{
	typedef game_camera super;
public:
						actor_input_controller	( game_scene& w );

	//input::handler stuff
	virtual	bool		on_keyboard_action	( input::world* input_world, input::enum_keyboard key, input::enum_keyboard_action action );
	virtual	bool		on_gamepad_action	( input::world* input_world, input::gamepad_button button, input::enum_gamepad_action action );
	virtual	bool		on_mouse_key_action	( input::world* input_world, input::mouse_button button, input::enum_mouse_key_action action );
	virtual	bool		on_mouse_move		( input::world* input_world, int x, int y, int z );
	virtual int			input_priority		( ) {return 10;}
	virtual	void		on_before_processing( input::world* world );
	virtual	void		on_after_processing	( input::world* world );


	float				onframe_move_fwd	( )	{ return m_frame_events.m_onframe_move_fwd; };
	float				onframe_move_right	( )	{ return m_frame_events.m_onframe_move_right; };
	float				onframe_turn_y		( )	{ return m_frame_events.m_onframe_turn_y; };
	float				onframe_turn_x		( )	{ return m_frame_events.m_onframe_turn_x; };
	bool				onframe_jump		( )	{ return m_frame_events.m_onframe_jump; };
	bool				on_frame_fire		( );
	bool				on_frame_sprint		( )	{ return m_sprint_toggle; }
	
	float				last_frame_time_delta( ) const { return m_frame_events.m_last_frame_time_delta; };

	// game_camera stuff
	virtual void	on_focus			( bool b_focus_enter);

private:
	static void			update_camera_matrix( frame_events const& frame_events, math::float4x4& camera_matrix );

	 frame_events				m_frame_events;
public:
	timing::timer				m_timer;
	bool						m_sprint_toggle;
};

} // namespace stalker2
#endif // #ifndef ACTOR_INPUT_CONTROLLER_H_INCLUDED
