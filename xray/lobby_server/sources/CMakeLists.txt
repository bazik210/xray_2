project(lobby_server LANGUAGES CXX)

add_executable(lobby_server
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/../../login_server/constants.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/library_linkage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/../../login_server/message_types.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					# server
					"${CMAKE_CURRENT_SOURCE_DIR}/lobby_server.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/lobby_server.h"
					# client_session
					"${CMAKE_CURRENT_SOURCE_DIR}/client_session.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/client_session.h"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/entry_point.cpp"
)

set_target_properties(lobby_server PROPERTIES FOLDER "engine/network")
