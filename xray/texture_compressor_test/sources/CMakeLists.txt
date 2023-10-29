project(texture_compressor_test LANGUAGES CXX)

add_executable(texture_compressor_test
					# application
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_compressor_test_application.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_compressor_test_application.h"
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/math_error.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_compressor_test_library_linkage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_compressor_test_memory.h"
					# resources
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_compressor_test.ico"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_compressor_test.rc"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_compressor_test_resources.h"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_compressor_test_entry_point.cpp"
)

target_link_libraries(texture_compressor_test PRIVATE
	core
)

set_target_properties(texture_compressor_test PROPERTIES FOLDER "engine/utilities")
