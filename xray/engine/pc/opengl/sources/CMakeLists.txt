project(engine_pc_opengl LANGUAGES CXX)

add_library(engine_pc_opengl SHARED
								# include
								"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
								# ./
								"${CMAKE_CURRENT_SOURCE_DIR}/../../engine_pc_entry_point.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/../../linkage_resolver.h"
)

target_link_libraries(engine_pc_opengl PRIVATE
	bugtrap
	renderer_engine_pc_opengl
	ui
	renderer_facade
	collision
	ai_navigation
	vfs
	sound
	vorbisfile
	physics
	bullet
	script_cs
	particle
	rtp
	network
	ai
	animation
	input
	vorbis
)

set_target_properties(engine_pc_opengl PROPERTIES FOLDER "engine")
