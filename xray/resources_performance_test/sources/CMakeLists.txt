project(resources_performance_test LANGUAGES CXX)

add_executable(resources_performance_test
					# application
					"${CMAKE_CURRENT_SOURCE_DIR}/resources_performance_test_application.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/resources_performance_test_application.h"
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/math_error.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/resources_performance_test_library_linkage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/resources_performance_test_memory.h"
					# resources
					"${CMAKE_CURRENT_SOURCE_DIR}/resources_performance_test.ico"
					"${CMAKE_CURRENT_SOURCE_DIR}/resources_performance_test.rc"
					"${CMAKE_CURRENT_SOURCE_DIR}/resources_performance_test_resources.h"
					# My Amplifier Results
					# MISSING
					#"${CMAKE_CURRENT_SOURCE_DIR}/My Amplifier Results/r000hs/r000hs.ampl"
					#"${CMAKE_CURRENT_SOURCE_DIR}/My Amplifier Results/r001hs/r001hs.ampl"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/resources_performance_test_entry_point.cpp"
)

set_target_properties(resources_performance_test PROPERTIES FOLDER "engine/utilities")
