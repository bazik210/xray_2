project(shader_compiler LANGUAGES CXX)

add_executable(shader_compiler
				# application
				"${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler_application.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler_application.h"
				# compiler
				# define_manager
				"${CMAKE_CURRENT_SOURCE_DIR}/define_manager.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/define_manager.h"
				# file_batcher
				"${CMAKE_CURRENT_SOURCE_DIR}/file_batcher.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/file_batcher.h"
				# resource_parser
				"${CMAKE_CURRENT_SOURCE_DIR}/resource_parser.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/resource_parser.h"
				# compiler
				"${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler.h"
				# utility
				"${CMAKE_CURRENT_SOURCE_DIR}/utility_functions.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/utility_functions.h"
				# command_line_parameters
				"${CMAKE_CURRENT_SOURCE_DIR}/command_line_parameters.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/command_line_parameters.h"
				# core
				"${CMAKE_CURRENT_SOURCE_DIR}/math_error.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler_library_linkage.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler_memory.h"
				# resources
				"${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler.ico"
				"${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler.rc"
				"${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler_resources.h"
				# ./
				"${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler_entry_point.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler_flags.txt"
)

set_target_properties(shader_compiler PROPERTIES FOLDER "engine/utilities")
