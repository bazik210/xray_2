project(vfs_pack LANGUAGES CXX)

add_executable(vfs_pack
				# core
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/vfs_pack_library_linkage.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/vfs_pack_memory.h"
				# application
				"${CMAKE_CURRENT_SOURCE_DIR}/vfs_pack.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/vfs_pack_application.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/vfs_pack_application.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/vfs_pack_entry_point.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/vfs_pack_pack.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/vfs_pack_patch.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/vfs_pack_unpack.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/vfs_pack_utils.cpp"
				# ./
)

set_target_properties(vfs_pack PROPERTIES FOLDER "engine/utilities")
