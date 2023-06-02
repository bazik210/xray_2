project(test_animations LANGUAGES CXX)

add_executable(test_animations
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/math_error_win.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/test_animations_library_linkage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/test_animations_memory.h"
					# application
					"${CMAKE_CURRENT_SOURCE_DIR}/test_animations_application.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/test_animations_application.h"
					# skeleton
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/test_animations_entry_point.cpp"
)

target_link_libraries(test_animations PRIVATE
	core
	animation
)

set_target_properties(test_animations PROPERTIES FOLDER "engine/utilities")
