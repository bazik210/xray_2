project(engine_pc_dx11 LANGUAGES CXX)

add_library(engine_pc_dx11 STATIC
							# include
							"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
							# ./
							"${CMAKE_CURRENT_SOURCE_DIR}/../../engine_pc_entry_point.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/../../linkage_resolver.h"
)

target_include_directories(engine_pc_dx11 PRIVATE
    ${SOLUTION_DIR_INCLUDE_PATH}
	${THIRDPARTY_DIR_INCLUDE_PATH}
)
target_link_directories(engine_pc_dx11 PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/binaries
	${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9.x64
)
target_link_libraries(engine_pc_dx11 PRIVATE
	odbc32.lib
	odbccp32.lib
	zlibN
	bugtrap
	script_cs
	core_cs
	lua.5.1.4
	luabind0.8.1
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
	particle
	maya_animation
	rtp
	network
	ai
	animation
	input
	logging
	vorbis
	debugging
	renderer_core_pc_dx11
	renderer_engine_pc_dx11
	texture_compressor
)

set_target_properties(engine_pc_dx11 PROPERTIES FOLDER "engine")
