add_library(debug STATIC
	# core
	"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
	# include
	"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../call_stack.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../check_invariants.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../check_no_multithread.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../check_no_multithread_inline.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../debug.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../debug_pointers.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../extensions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../log_callback.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../macros.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../macros_inline.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../pointer_cast.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../signalling_bool.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../static_cast_checked.h"
	# bugtrap
	"${CMAKE_CURRENT_SOURCE_DIR}/bugtrap.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/bugtrap_win.cpp"
	# callstack
	"${CMAKE_CURRENT_SOURCE_DIR}/call_stack.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/call_stack_win.cpp"
	# initialize
	"${CMAKE_CURRENT_SOURCE_DIR}/float.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/initialize.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/initialize_win.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/initialize_win_xbox360.cpp"
	# unhandled
	"${CMAKE_CURRENT_SOURCE_DIR}/unhandled_win.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/unhandled_win_xbox360.cpp"
	# ./
	"${CMAKE_CURRENT_SOURCE_DIR}/debug.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/debug_win.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/debug_win_xbox360.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/macros.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/platform.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/utils.h"
)

# Put library to xray namespace
add_library(xray::debug ALIAS debug)

target_include_directories(debug PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../../")
target_include_directories(debug PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../")
target_include_directories(debug PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")

set_target_properties(debug PROPERTIES FOLDER "engine/core")
