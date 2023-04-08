////////////////////////////////////////////////////////////////////////////
//	Created		: 16.03.2012
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "lobby_menu.h"

namespace stalker2{

bool lobby_menu::on_keyboard_action		( input::world* input_world, input::enum_keyboard key, input::enum_keyboard_action action )
{
	return false;
}
bool lobby_menu::on_gamepad_action		( input::world* input_world, input::gamepad_button button, input::enum_gamepad_action action )
{
	return false;
}
bool lobby_menu::on_mouse_key_action		( input::world* input_world, input::mouse_button button, input::enum_mouse_key_action action )
{
	return false;
}
bool lobby_menu::on_mouse_move			( input::world* input_world, int x, int y, int z )
{
	return false;
}
}