project(rms_generator LANGUAGES CXX)

add_executable(rms_generator
				# core
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/rms_generator_library_linkage.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/rms_generator_memory.h"
				# application
				"${CMAKE_CURRENT_SOURCE_DIR}/rms_generator_application.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/rms_generator_application.h"
				# rms_evaluator
				"${CMAKE_CURRENT_SOURCE_DIR}/rms_evaluator.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/rms_evaluator.h"
				# ./
				"${CMAKE_CURRENT_SOURCE_DIR}/rms_generator_entry_point.cpp"
)

target_link_libraries(rms_generator PRIVATE
	core
	ogg
	vorbis
)

set_target_properties(rms_generator PROPERTIES FOLDER "engine/utilities")
