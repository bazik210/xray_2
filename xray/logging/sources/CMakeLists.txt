project(logging LANGUAGES CXX)

add_library(logging STATIC
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/logging_library_linkage.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/logging_memory.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						# include
						"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../extensions.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../format.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../format_specifier.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../helper.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../log_file.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../macros.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../noncopyable.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
						# helper
						"${CMAKE_CURRENT_SOURCE_DIR}/format_specifier.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/helper.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/helper_win_xbox360.cpp"
						# path_parts
						"${CMAKE_CURRENT_SOURCE_DIR}/path_parts.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/path_parts.h"
						# log_file
						"${CMAKE_CURRENT_SOURCE_DIR}/log_file.cpp"
						# filter_tree
						"${CMAKE_CURRENT_SOURCE_DIR}/filter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/filter_tree.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/filter_tree_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/filter_tree_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/filter_tree_node_base.h"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/globals.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/globals.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/logging.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/logging.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/logging_win.cpp"
)

set_target_properties(logging PROPERTIES FOLDER "engine/core")
