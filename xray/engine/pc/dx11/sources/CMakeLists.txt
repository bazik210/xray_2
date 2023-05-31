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
)

set_target_properties(engine_pc_dx11 PROPERTIES FOLDER "engine")
