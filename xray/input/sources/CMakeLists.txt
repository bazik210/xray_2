project(input LANGUAGES CXX)

add_library(input STATIC
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/input_library_linkage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/input_memory.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					# include
					"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../gamepad.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../handler.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../keyboard.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../mouse.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
					# world
					"${CMAKE_CURRENT_SOURCE_DIR}/input_world.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/input_world.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/input_world_win.cpp"
					# receivers
					"${CMAKE_CURRENT_SOURCE_DIR}/direct_input_include.h"
					# mouse
					"${CMAKE_CURRENT_SOURCE_DIR}/receiver_mouse.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/receiver_mouse.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/receiver_mouse_win.cpp"
					# keyboard
					"${CMAKE_CURRENT_SOURCE_DIR}/receiver_keyboard.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/receiver_keyboard.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/receiver_keyboard_win.cpp"
					# gamepad
					"${CMAKE_CURRENT_SOURCE_DIR}/receiver_gamepad.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/receiver_gamepad_win_xbox360.cpp"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/input_entry_point.cpp"
)

target_link_libraries(input PRIVATE
	engine_pc
	dxguid.lib
	dinput8.lib
)

set_target_properties(input PROPERTIES FOLDER "engine")
