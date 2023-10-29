project(client LANGUAGES CXX)

add_executable(client
					# core
					#"${CMAKE_CURRENT_SOURCE_DIR}/../constants.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/library_linkage.cpp"
					#"${CMAKE_CURRENT_SOURCE_DIR}/../login_structures.h"
					#"${CMAKE_CURRENT_SOURCE_DIR}/../message_types.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					# client
					"${CMAKE_CURRENT_SOURCE_DIR}/client.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/client.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/connection_info.h"
					# chat_client
					# lobby_client
					"${CMAKE_CURRENT_SOURCE_DIR}/lobby_client.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/lobby_client.h"
					# login_client
					"${CMAKE_CURRENT_SOURCE_DIR}/login_client.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/login_client.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/login_client_connect.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/login_client_handshake.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/login_client_ping.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/login_client_resolve.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/login_client_sign_in.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/login_client_sign_out.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/login_client_sign_up.cpp"
					# match_client
					"${CMAKE_CURRENT_SOURCE_DIR}/match_client.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/match_client.h"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/entry_point.cpp"
)

target_link_libraries(client PRIVATE
	odbc32.lib
	odbcpp32.lib
	libeay32-vc90-mt-s.lib
	ssleay32-vc90-mt-s.lib
)

set_target_properties(client PROPERTIES FOLDER "engine/network")
