project(login_server LANGUAGES CXX)

add_executable(login_server
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/../constants.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/delayhlp.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/library_linkage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/memory_allocator.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../message_types.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					# server
					"${CMAKE_CURRENT_SOURCE_DIR}/login_server.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/login_server.h"
					# query_processor
					"${CMAKE_CURRENT_SOURCE_DIR}/query_processor.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/query_processor.h"
					# client_session
					"${CMAKE_CURRENT_SOURCE_DIR}/client_session.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/client_session.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/client_session_sign_in.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/client_session_sign_out.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/client_session_sign_up.cpp"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/entry_point.cpp"
)

target_link_libraries(login_server PRIVATE
	odbc32.lib
	odbcpp32.lib
	libmysql.dll
	ws2_32.dll
	mswsock.dll
	libeay32-vc90-mt-s.lib
	ssleay32-vc90-mt-s.lib
)

set_target_properties(login_server PROPERTIES FOLDER "engine/network")
