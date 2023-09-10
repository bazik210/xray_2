project(engine LANGUAGES CXX)

add_library(engine STATIC
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_library_linkage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_memory.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					# include
					"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../engine_user.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../library_dependencies.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
					# world
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_scaleform_initialize.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_world.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_world.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_world_build.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_world_editor.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_world_file_system_device.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_world_initialize.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_world_interfaces.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_world_logic.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_world_network.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_world_resources.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_world_sound.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_world_terminate_on_timeout.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_world_win.cpp"
					# console
					"${CMAKE_CURRENT_SOURCE_DIR}/../console.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/console_impl.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/console_impl.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/console_impl_input.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_console.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_console.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_console.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_console.h"
					# apc
					"${CMAKE_CURRENT_SOURCE_DIR}/apc.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/apc.h"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_entry_point.cpp"
)

target_include_directories(engine PRIVATE
#	${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Include
#	${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Src
	"${DXSDK_DIR_PATH}/Include"
)

target_link_directories(engine PUBLIC
#	${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Lib/x64/Msvc90/Debug
	"${DXSDK_DIR_PATH}/Lib/x64"
)

target_link_libraries(engine PUBLIC
#	scaleform
	delayimp.lib
)

set_target_properties(engine PROPERTIES FOLDER "engine")

set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/engine_scaleform_initialize.cpp PROPERTY VS_SETTINGS "ExcludedFromBuild=true")
