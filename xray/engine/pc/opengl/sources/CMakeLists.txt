project(engine_pc_opengl LANGUAGES CXX)

add_library(engine_pc_opengl STATIC
								# include
								"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
								# ./
								"${CMAKE_CURRENT_SOURCE_DIR}/../../engine_pc_entry_point.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/../../linkage_resolver.h"
)

set_target_properties(engine_pc_opengl PROPERTIES FOLDER "engine")
