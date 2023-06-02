project(network LANGUAGES CXX)

add_library(network STATIC
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/handler_allocator.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/network_library_linkage.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/network_memory.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						# world
						"${CMAKE_CURRENT_SOURCE_DIR}/network_world.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/network_world.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/two_way_threads_channel.h"
						# client
						"${CMAKE_CURRENT_SOURCE_DIR}/client.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/client_error_codes.h"
						# impl
						"${CMAKE_CURRENT_SOURCE_DIR}/client_impl.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/client_impl.h"
						# server
						"${CMAKE_CURRENT_SOURCE_DIR}/server.cpp"
						# impl
						"${CMAKE_CURRENT_SOURCE_DIR}/server_impl.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/server_impl.h"
						# client_session
						"${CMAKE_CURRENT_SOURCE_DIR}/client_session.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/client_session.h"
						# packet_socket
						"${CMAKE_CURRENT_SOURCE_DIR}/packet_socket.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/packet_socket_inline.h"
						# packet
						"${CMAKE_CURRENT_SOURCE_DIR}/packet.cpp"
						# orders
						"${CMAKE_CURRENT_SOURCE_DIR}/connect_order.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/functor_order.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/order.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/send_order.h"
						# responses
						"${CMAKE_CURRENT_SOURCE_DIR}/functor_response.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/receive_response.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/response.h"
						# include
						"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../client.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../server.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
						# packet
						"${CMAKE_CURRENT_SOURCE_DIR}/../packet.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../packet_inline.h"
						# packet_reader
						"${CMAKE_CURRENT_SOURCE_DIR}/../packet_reader.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../packet_reader_inline.h"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/network_entry_point.cpp"
)

target_link_libraries(network PRIVATE
	core
	debugging
	logging
)

set_target_properties(network PROPERTIES FOLDER "engine/network")
