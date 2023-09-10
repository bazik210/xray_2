////////////////////////////////////////////////////////////////////////////
//	Created		: 24.03.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef MAIN_MENU_H_INCLUDED
#define MAIN_MENU_H_INCLUDED

#include <xray/input/handler.h>
#include "game_scene.h"

struct sc_support;
namespace stalker2 {

struct main_menu_ui;
class game;

#ifdef XRAY_RENDERER_FLASH
struct flash_movie;
struct ui_mouse_position
{
public:
	ui_mouse_position( u32 init_x = 0, u32 init_y = 0 ): x(init_x), y(init_y){}
	int x;
	int y;
};
#endif //#ifdef XRAY_RENDERER_FLASH


class main_menu :
	public game_scene,
	public xray::input::handler
{
	typedef			game_scene					super;

public:
					main_menu				( game& g, game_world& w );
	virtual			~main_menu				( );
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
	
private:
#ifdef XRAY_RENDERER_FLASH
	void			create_main_menu_ui		();
#else	
	input::handler*	dialog_input_handler	();
#endif //#ifdef XRAY_STATIC_LIBRARIES

private:
			void	query_resources			( );
			void	on_resources_ready		( xray::resources::queries_result& data );

#ifdef XRAY_RENDERER_FLASH
	flash_movie*	m_main_menu_ui;
	timing::timer	m_timer;
	u32				m_main_menu_ui_last_time;
	ui_mouse_position m_mouse_pos;
	math::uint2		m_window_size;
#else
	main_menu_ui*	m_ui;
#endif //#ifdef XRAY_RENDERER_FLASH

	game_world&		m_game_world;

}; // class main_menu

} // namespace stalker2

#endif // #ifndef MAIN_MENU_H_INCLUDED