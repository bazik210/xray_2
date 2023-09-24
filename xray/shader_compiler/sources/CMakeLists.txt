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

target_include_directories(shader_compiler PRIVATE
    ${SOLUTION_DIR_INCLUDE_PATH}
    ${THIRDPARTY_DIR_INCLUDE_PATH}
    ${DXSDK_DIR_PATH}/Include
)

if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    # set platform as x64
	target_compile_definitions(shader_compiler PRIVATE
	  PLATFORM_WIN64
          WIN64
          WINDOWS
          _HAS_EXCEPTIONS=0
	  BOOST_NO_EXCEPTIONS
    )
else()
    # set platform as x86
	target_compile_definitions(shader_compiler PRIVATE
	  PLATFORM_WIN32
          WIN32
          WINDOWS
          _HAS_EXCEPTIONS=0
	  BOOST_NO_EXCEPTIONS
    )
endif()

target_link_libraries(shader_compiler PRIVATE
	fs
	vfs
	core
	renderer_core_pc_dx11
)

if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
target_link_directories(shader_compiler PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/binaries
	${DXSDK_DIR_PATH}/Lib/x64
)
else()
target_link_directories(shader_compiler PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/binaries
	${DXSDK_DIR_PATH}/Lib/x86
)
endif()

# set compiler flags
target_compile_options(shader_compiler PRIVATE
    /GS-
    /MD
    /EHsc
)

set_target_properties(shader_compiler PROPERTIES FOLDER "engine/utilities")
