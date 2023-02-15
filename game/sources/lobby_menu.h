////////////////////////////////////////////////////////////////////////////
//	Created		: 16.03.2012
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef LOBBY_MENU_H_INCLUDED
#define LOBBY_MENU_H_INCLUDED

#include <xray/input/handler.h>
#include <xray/render/facade/model.h>
#include "game_scene.h"
#include "game_project.h"

namespace stalker2{

class game;
class free_fly_camera;

class lobby_menu :	public game_scene,
					public xray::input::handler

{
	typedef			game_scene				super;
public:
					lobby_menu				( game& w );
	virtual			~lobby_menu				( );

public:
	virtual void	on_activate				( );
	virtual void	on_deactivate			( );
	virtual void	tick					( );

	virtual	xray::input::handler& input_handler	( ) { return *this; }

	virtual	bool	on_keyboard_action		( input::world* input_world, input::enum_keyboard key, input::enum_keyboard_action action );
	virtual	bool	on_gamepad_action		( input::world* input_world, input::gamepad_button button, input::enum_gamepad_action action );
	virtual	bool	on_mouse_key_action		( input::world* input_world, input::mouse_button button, input::enum_mouse_key_action action );
	virtual	bool	on_mouse_move			( input::world* input_world, int x, int y, int z );
	virtual int		input_priority			( ) { return 100; }
			void	clear_resources			( );
	
private:
			void	query_resources			( );
			void	on_render_scenes_ready	( xray::resources::queries_result& data );
			void	on_game_project_ready	( xray::resources::queries_result& data );

	camera_director_ptr						m_camera_director;
	game_camera*							m_game_camera;

	simple_game_project_ptr					m_lobby_game_project;
}; // class lobby_menu

} // namespace stalker2

#endif // #ifndef LOBBY_MENU_H_INCLUDED