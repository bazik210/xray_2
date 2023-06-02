project(rtp_learn LANGUAGES CXX)

add_executable(rtp_learn
				# application
				"${CMAKE_CURRENT_SOURCE_DIR}/rtp_learn_application.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/rtp_learn_application.h"
				# core
				"${CMAKE_CURRENT_SOURCE_DIR}/math_error.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/rtp_learn_library_linkage.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/rtp_learn_memory.h"
				# resources
				"${CMAKE_CURRENT_SOURCE_DIR}/rtp_learn.ico"
				"${CMAKE_CURRENT_SOURCE_DIR}/rtp_learn.rc"
				"${CMAKE_CURRENT_SOURCE_DIR}/rtp_learn_resources.h"
				# ./
				"${CMAKE_CURRENT_SOURCE_DIR}/rtp_learn_entry_point.cpp"
)

target_link_libraries(rtp_learn PRIVATE
	core
	rtp
	animation
)

set_target_properties(rtp_learn PROPERTIES FOLDER "engine/utilities")
