project(fs_pack LANGUAGES CXX)

add_executable(fs_pack
				# core
				"${CMAKE_CURRENT_SOURCE_DIR}/fs_pack_library_linkage.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/fs_pack_memory.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
				# application
				"${CMAKE_CURRENT_SOURCE_DIR}/fs_pack_application.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/fs_pack_application.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/fs_pack_entry_point.cpp"
				# ./
)

target_link_libraries(fs_pack PRIVATE
	fs
	script_cs
)

set_target_properties(fs_pack PROPERTIES FOLDER "engine/utilities")