project(sound_system_usage_example LANGUAGES CXX)

add_executable(sound_system_usage_example
							# application
							"${CMAKE_CURRENT_SOURCE_DIR}/npc.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/npc.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_system_usage_example_application.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_system_usage_example_application.h"
							# core
							"${CMAKE_CURRENT_SOURCE_DIR}/math_error.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_system_usage_example_library_linkage.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_system_usage_example_memory.h"
							# resources
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_system_usage_example.ico"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_system_usage_example.rc"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_system_usage_example_resources.h"
							# ./
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_system_usage_example_entry_point.cpp"
)

target_link_libraries(sound_system_usage_example PRIVATE
	vfs
	engine
	vorbis
	ode
	render_core_pc_dx11
	particle
	sound
	collision
	animation
)

set_target_properties(sound_system_usage_example PROPERTIES FOLDER "engine/utilities")
