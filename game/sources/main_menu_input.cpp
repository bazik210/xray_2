////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "main_menu.h"
#include "game.h"
#include <xray/input/world.h>
#include <xray/input/keyboard.h>
#include <xray/input/mouse.h>
#include <xray/input/gamepad.h>
#include "game_world.h"
#include "key_binder.h"

#ifdef XRAY_STATIC_LIBRARIES
#include "GFx.h"
#include "flash_factory.h"
#endif //#ifdef XRAY_STATIC_LIBRARIES

namespace stalker2{

bool main_menu::on_keyboard_action( input::world* input_world, input::enum_keyboard key, input::enum_keyboard_action action )
{

	XRAY_UNREFERENCED_PARAMETER( input_world );
	if(action==xray::input::kb_key_down)
	{
		game_action_id act = stalker2::key_binder::get_binded_action( key );
		LOG_INFO( " [%s] key pressed [%s]", key_binder::dik_to_keyname( key ), key_binder::id_to_action_name( act ) );

 		if(key == xray::input::key_q)
 		{
 			get_game().exit		( "quit" );
 			return				true;
 		}
 
 		if(key == xray::input::key_h)
 		{
 			m_game_world.switch_to_hud_camera();
			get_game().scene_close_query();
 			return				true;
 		}
 
 		if(key == xray::input::key_f)
 		{
 			m_game_world.switch_to_free_fly_camera();
			get_game().scene_close_query();
 			return				true;
 		}
	}

#ifdef XRAY_STATIC_LIBRARIES
		return false;	
#else
	return dialog_input_handler()->on_keyboard_action( input_world, key, action );
#endif //#ifdef XRAY_STATIC_LIBRARIES
}

bool main_menu::on_gamepad_action( input::world* input_world, input::gamepad_button button, input::enum_gamepad_action action )
{
#ifdef XRAY_STATIC_LIBRARIES
	XRAY_UNREFERENCED_PARAMETERS( input_world, button, action );
	return false;	
#else
	return dialog_input_handler()->on_gamepad_action( input_world, button, action );
#endif //#ifdef XRAY_STATIC_LIBRARIES
}

bool main_menu::on_mouse_key_action( input::world* input_world, input::mouse_button button, input::enum_mouse_key_action action )
{
#ifdef XRAY_STATIC_LIBRARIES

	XRAY_UNREFERENCED_PARAMETERS( input_world, button, action );

	Scaleform::GFx::Event::EventType ev_type = Scaleform::GFx::Event::Unknown;
	if ( action == input::ms_key_down )
		ev_type = Scaleform::GFx::Event::MouseDown;
	else if ( action == input::ms_key_up )
		ev_type = Scaleform::GFx::Event::MouseUp;

	Scaleform::GFx::MouseEvent mevent( ev_type, 0, (float)m_mouse_pos.x, (float)m_mouse_pos.y );
	m_main_menu_ui->m_movie->HandleEvent(mevent);
	return true;	
#else
	return dialog_input_handler()->on_mouse_key_action( input_world, button, action );
#endif //#ifdef XRAY_STATIC_LIBRARIES
}

bool main_menu::on_mouse_move( input::world* input_world, int x, int y, int z )
{
#ifdef XRAY_STATIC_LIBRARIES

	XRAY_UNREFERENCED_PARAMETERS( input_world, z );

	m_mouse_pos.x += x;
	m_mouse_pos.y += y;

	math::clamp<int>(m_mouse_pos.x, 0, m_window_size.width);
	math::clamp<int>(m_mouse_pos.y, 0, m_window_size.height);

	//	LOG_INFO("on_mouse_move[%d]:[%d]", m_mouse_pos.x,m_mouse_pos.y);
	Scaleform::GFx::MouseEvent mevent( Scaleform::GFx::Event::MouseMove, 0, (float)m_mouse_pos.x, (float)m_mouse_pos.y );
	m_main_menu_ui->m_movie->HandleEvent(mevent);
	return true;
#else
	return dialog_input_handler()->on_mouse_move( input_world, x, y, z );
#endif //#ifdef XRAY_STATIC_LIBRARIES
}

} // namespace stalker2

