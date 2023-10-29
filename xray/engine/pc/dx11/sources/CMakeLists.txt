project(engine_pc_dx11 LANGUAGES CXX)

add_library(engine_pc_dx11 SHARED
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

if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
target_link_directories(engine_pc_dx11 PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/binaries
	${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9.x64
	${DXSDK_DIR_PATH}/Lib/x64
)
else()
target_link_directories(engine_pc_dx11 PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/binaries
	${THIRDPARTY_DIR_INCLUDE_PATH}/SpeedTree/Lib/Windows/VC9
	${DXSDK_DIR_PATH}/Lib/x86
)
endif()

target_link_libraries(engine_pc_dx11 PUBLIC
	zlibN
	bugtrap
	script_cs
	ui
	vorbisfile
	bullet
	particle
	vorbis
	FreeImage
	renderer_engine_pc_dx11
	texture_compressor
	engine_pc
)

set_target_properties(engine_pc_dx11 PROPERTIES FOLDER "engine")

target_link_libraries(engine_pc_dx11 PRIVATE legacy_stdio_definitions.lib)
