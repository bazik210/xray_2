project(fs LANGUAGES CXX)

add_library(fs STATIC
				# core
				"${CMAKE_CURRENT_SOURCE_DIR}/library_linkage.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/memory.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
				# include
				"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../platform_configuration.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../simple_asynchronous_interface.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../simple_synchronous_interface.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
				# device_file_systems
				"${CMAKE_CURRENT_SOURCE_DIR}/../asynchronous_device_commands.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../asynchronous_device_interface.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../device_file_system_interface.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../device_file_system_no_watcher.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../device_file_system_proxy.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../device_file_system_proxy_base.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../device_file_system_watcher.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../device_utils.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../synchronous_device_interface.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../windows_hdd_file_system.h"
				# path_strings
				"${CMAKE_CURRENT_SOURCE_DIR}/../native_path_string.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../path_part_iterator.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../path_part_iterator_inline.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../path_string_impl.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../path_string_impl_inline.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../path_string_utils.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../path_string_utils_inline.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../portable_path_string.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../virtual_path_string.h"
				# physical_path_info
				"${CMAKE_CURRENT_SOURCE_DIR}/../physical_path_info.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../physical_path_info_data.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../physical_path_iterator.h"
				# file_type
				"${CMAKE_CURRENT_SOURCE_DIR}/../file_type.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../file_type_pointer.h"
				# watcher
				"${CMAKE_CURRENT_SOURCE_DIR}/../folder_watcher.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../watcher.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../watcher_notification.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../watcher_subscriber.h"
				"${CMAKE_CURRENT_SOURCE_DIR}/../watcher_utils.h"
				# asynchronous_device_interface
				"${CMAKE_CURRENT_SOURCE_DIR}/asynchronous_device_commands.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/asynchronous_device_interface.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/asynchronous_device_interface_commands.cpp"
				# path_strings
				"${CMAKE_CURRENT_SOURCE_DIR}/path_string_impl.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/path_string_utils.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/path_string_utils_win.cpp"
				# physical_path_info
				"${CMAKE_CURRENT_SOURCE_DIR}/physical_path_info_iterator.cpp"
				# watcher
				"${CMAKE_CURRENT_SOURCE_DIR}/folder_watcher.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/watcher.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/watcher_notification.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/watcher_subscriber.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/watcher_utils.cpp"
				# windows_hdd_file_system
				"${CMAKE_CURRENT_SOURCE_DIR}/windows_hdd_file_system.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/windows_hdd_file_system_iteration.cpp"
				# simple_interfaces
				"${CMAKE_CURRENT_SOURCE_DIR}/simple_asynchronous_interface.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/../simple_synchronous_interace.cpp"
				# ./
				"${CMAKE_CURRENT_SOURCE_DIR}/device_file_system_no_watcher.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/device_file_system_proxy_base.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/device_file_system_watcher.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/device_utils.cpp"
				"${CMAKE_CURRENT_SOURCE_DIR}/synchronous_device_interface.cpp"
)

target_link_libraries(fs PUBLIC
	core
)

target_include_directories(fs PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/xray/fs
    ${SOLUTION_DIR_INCLUDE_PATH}/xray/fs/sources
)

set_target_properties(fs PROPERTIES FOLDER "engine/core")
