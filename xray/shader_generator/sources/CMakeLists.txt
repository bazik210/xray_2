project(shader_generator LANGUAGES CXX)

add_executable(shader_generator
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/shader_compiler.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/shader_generator_entry_point.cpp"
)

target_include_directories(shader_generator PRIVATE
    ${SOLUTION_DIR_INCLUDE_PATH}
    ${THIRDPARTY_DIR_INCLUDE_PATH}
    ${DXSDK_DIR_PATH}/Include
)


if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    # set platform as x64
	target_compile_definitions(shader_generator PRIVATE
	  PLATFORM_WIN64
          WIN64
          WINDOWS
    )
else()
    # set platform as x86
	target_compile_definitions(shader_generator PRIVATE
	  PLATFORM_WIN32
          WIN32
          WINDOWS
    )
endif()

if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
target_link_directories(shader_generator PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/binaries
	${DXSDK_DIR_PATH}/Lib/x64
)
else()
target_link_directories(shader_generator PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/binaries
	${DXSDK_DIR_PATH}/Lib/x86
)
endif()

# set compiler flags
target_compile_options(shader_generator PRIVATE
    /GS-
    /MD
    /EHsc
)

target_link_libraries(shader_generator PRIVATE
#	odbc32.lib
#	odbcpp32.lib
)

get_target_property(defs shader_generator COMPILE_DEFINITIONS)
list(FILTER defs EXCLUDE REGEX [[^BOOST_NO_EXCEPTIONS$]])
set_property(TARGET shader_generator PROPERTY COMPILE_DEFINITIONS ${defs})

set_target_properties(shader_generator PROPERTIES FOLDER "engine/utilities")
