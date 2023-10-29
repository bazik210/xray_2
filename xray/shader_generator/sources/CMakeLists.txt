project(shader_generator LANGUAGES CXX)

add_executable(shader_generator
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/shader_generator_entry_point.cpp"
)

target_link_libraries(shader_generator PRIVATE
	odbc32.lib
	odbcpp32.lib
)

set_target_properties(shader_generator PROPERTIES FOLDER "engine/utilities")
