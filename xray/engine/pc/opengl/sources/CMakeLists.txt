project(engine_pc_opengl LANGUAGES CXX)

add_library(engine_pc_opengl STATIC
								# include
								"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
								# ./
								"${CMAKE_CURRENT_SOURCE_DIR}/../../engine_pc_entry_point.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/../../linkage_resolver.h"
)

target_link_libraries(engine_pc_opengl PRIVATE
	odbc32.lib
	odbccp32.lib
	bugtrap
	renderer_engine_pc_opengl
	engine
	ogg
	ui
	ode
	renderer_facade
	collision
	ai_navigation
	fs
	vfs
	opcode
	sound
	vorbisfile
	physics
	bullet
	core
	script_cs
	core_cs
	lua.5.1.4
	luabind.0.8.1
	particle
	maya_animation
	rtp
	network
	ai
	animation
	input
	logging
	debugging
	vorbis
)

set_target_properties(engine_pc_opengl PROPERTIES FOLDER "engine")
