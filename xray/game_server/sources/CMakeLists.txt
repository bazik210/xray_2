project(game_server LANGUAGES CXX)

add_executable(game_server
					# application
					"${CMAKE_CURRENT_SOURCE_DIR}/game_server_application.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_server_application.h"
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/game_server_library_linkage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_server_memory.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/math_error.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					# resources
					"${CMAKE_CURRENT_SOURCE_DIR}/game_server.ico"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_server.rc"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_server_resources.h"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/game_server_entry_point.cpp"
)

target_link_libraries(game_server PRIVATE
	core
	debugging
	logging
	fs
	vfs
	network
)

set_target_properties(game_server PROPERTIES FOLDER "engine/network")
