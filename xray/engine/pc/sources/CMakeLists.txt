project(engine_pc LANGUAGES CXX)

add_library(engine_pc SHARED
					# include
					"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/engine_pc_entry_point.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/linkage_resolver.h"
)

target_link_libraries(engine_pc PUBLIC
	engine
)

set_target_properties(engine_pc PROPERTIES FOLDER "engine")