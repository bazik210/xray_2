project(test LANGUAGES CXX)

add_executable(test
				# core
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/test_library_linkage.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/test_memory.h"
				# application
				"${CMAKE_CURRENT_SOURCE_DIR}/test_application.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/test_vfs.cpp"
				# include
				# ./
				"${CMAKE_CURRENT_SOURCE_DIR}/test_entry_point.cpp"
)

set_target_properties(test PROPERTIES FOLDER "engine/utilities")
