project(core LANGUAGES CXX)

add_library(core STATIC
	# core
	"${CMAKE_CURRENT_SOURCE_DIR}/core_library_linkage.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
	# extensions
	# memory
	"${CMAKE_CURRENT_SOURCE_DIR}/memory.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/memory.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_platform.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_video_win.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_win.cpp"
	# allocators
	# wrappers
	# dlmalloc
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_doug_lea_allocator.cpp"
	# ptmalloc3
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_pthreads3_allocator.cpp"
	# base
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_base_allocator.cpp"
	# stack
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_stack_allocator.cpp"
	# process
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_process_allocator.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_process_allocator.h"
	# dlmalloc_mt
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_doug_lea_mt_allocator.cpp"
	# crt
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_crt_allocator.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_crt_allocator_win.cpp"
	# ptmalloc3 & dlmalloc
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/malloc-2.8.3.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/malloc-private.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/malloc.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/out_of_memory_handler_types.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/ptmalloc3.c"
	# sysdeps
	# generic
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/sysdeps/generic/atomic.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/sysdeps/generic/malloc-machine.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/sysdeps/generic/thread-st.h"
	# pthread
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/sysdeps/pthread/malloc-machine.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/sysdeps/pthread/thread-st.h"
	# win32
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/sysdeps/win32/malloc-machine.h"
	# sproc
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/sysdeps/sproc/malloc-machine.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/sysdeps/sproc/thread-st.h"
	# solaris
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/sysdeps/solaris/malloc-machine.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/ptmalloc3/sysdeps/solaris/thread-st.h"
	# win_xbox360_ps3
	# virtual_alloc
	"${CMAKE_CURRENT_SOURCE_DIR}/virtual_alloc.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/virtual_alloc.h"
	# managed
	"${CMAKE_CURRENT_SOURCE_DIR}/managed_allocator.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/managed_allocator_base.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/managed_allocator_test_win_xbox360.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/managed_node.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/managed_node.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/managed_node_owner.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_allocators.h"
	# monitor
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_monitor.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_monitor.h"
	# reader
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_reader.cpp"
	# chunk
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_chunk_reader.cpp"
	# stream
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_stream.cpp"
	# leak_detector
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_leak_detector.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_leak_detector.h"
	# guard
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_guard.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_guard.h"
	# writer
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_writer.cpp"
	# operators
	"${CMAKE_CURRENT_SOURCE_DIR}/memory_override_operators.cpp"
	# threading
	"${CMAKE_CURRENT_SOURCE_DIR}/threading_active_object.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/threading_reader_writer_lock.cpp"
	# primitives
	# event
	"${CMAKE_CURRENT_SOURCE_DIR}/threading_event.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/threading_event_win_xbox360.cpp"
	# mutex
	"${CMAKE_CURRENT_SOURCE_DIR}/threading_mutex.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/threading_mutex_win_xbox360.cpp"
	# functions
	"${CMAKE_CURRENT_SOURCE_DIR}/threading_functions.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/threading_functions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/threading_functions_win.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/threading_functions_win_xbox360.cpp"
	# tests
	#"${CMAKE_CURRENT_SOURCE_DIR}/threading_test.cpp"
	# os_extensions
	# functions
	"${CMAKE_CURRENT_SOURCE_DIR}/os_functions_win.cpp"
	# strings
	"${CMAKE_CURRENT_SOURCE_DIR}/strings_functions.cpp"
	# shared
	# manager
	"${CMAKE_CURRENT_SOURCE_DIR}/strings_shared_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/strings_shared_manager.h"
	# profile
	"${CMAKE_CURRENT_SOURCE_DIR}/strings_shared_profile.cpp"
	# buffer
	"${CMAKE_CURRENT_SOURCE_DIR}/buffer_string.cpp"
	# concatenations
	# detail
	"${CMAKE_CURRENT_SOURCE_DIR}/strings_detail_tuples.cpp"
	# stream
	"${CMAKE_CURRENT_SOURCE_DIR}/strings_stream.cpp"
	# text_tree
	"${CMAKE_CURRENT_SOURCE_DIR}/text_tree.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/text_tree_item.cpp"
	# platform
	"${CMAKE_CURRENT_SOURCE_DIR}/delayhlp.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/platform_extensions_win.cpp"
	# fs
	# file system
	# macros
	# nodes
	# impl
	# saving
	# sub_fat_cook
	# disk_mount
	# finding
	# path
	# utils
	# watcher
	# resources
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_impl.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_impl.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_vfs_associations.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_vfs_associations.h"
	# memory
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_macros.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_memory.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_unmanaged_allocator.h"
	# device_managers
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_device_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_device_manager.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_device_manager_thread.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_hdd_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_hdd_manager.h"
	# test
	#"${CMAKE_CURRENT_SOURCE_DIR}/resources_test.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/resources_test_base_cook.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/resources_test_configuration.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/resources_test_inplace_managed.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/resources_test_inplace_unmanaged.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/resources_test_managed.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/resources_test_resource.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/resources_test_translate_query.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/resources_test_unmanaged.h"
	# helper
	"${CMAKE_CURRENT_SOURCE_DIR}/member_to_functor.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/member_to_functor_inline.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_helper.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_thread_local_data.h"
	# query_result
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_queries_result.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_query_result.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_query_result_allocation.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_query_result_cache.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_query_result_cook.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_query_result_finalization.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_query_result_initialization.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_query_result_path.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_query_result_replication.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_query_result_requery.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_save_generated_data.cpp"
	# cook
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_cook_base.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_cook_classes.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/unmanaged_allocation_cook.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/unmanaged_allocation_cook.h"
	# fs operations
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_fs.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_fs_iterator.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_fs_task.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_mount_ptr.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_query_fs_iterator.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_query_mount.cpp"
	# fs_task
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_fs_task.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_fs_task_composite.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_fs_task_composite.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_fs_task_erase.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_fs_task_erase.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_fs_task_iterator.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_fs_task_iterator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_fs_task_mount.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_fs_task_mount.h"
	# resource
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_intrusive_base.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_managed_resource.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_name_registry_entry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_resource_base.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_resource_children.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_resource_flags.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_resource_quality.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_resource_reconstruction_info.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_unmanaged_resource.cpp"
	# manager
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_manager.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_manager_allocation.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_manager_allocation.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_manager_cook.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_manager_deallocation.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_manager_debug.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_manager_debug.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_manager_helper.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_manager_new_queries.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_manager_user_thread.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_manager_watcher.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/resources_manager_watcher.h"
	# game_resman
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_interface.cpp"
	# test
	#"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_test.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_test_allocator.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_test_allocator.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_test_resource.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_test_resource.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_test_resource_cook.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_test_resource_cook.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_test_resource_with_quality.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_test_resource_with_quality.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_test_resource_with_quality_cook.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_test_resource_with_quality_cook.h"
	#"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_test_utils.cpp"
	#"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_test_utils.h"
	# satisfaction
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_satisfaction_tree.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_satisfaction_tree.h"
	# freeing
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_free.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_free.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_free_collection.h"
	# releasing
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_release.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_release.h"
	# quality
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_quality_decrease.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_quality_decrease.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_quality_increase.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_quality_increase.h"
	# manager
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_data.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_out_of_memory.cpp"
	# sorting
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_sorting.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/game_resman_sorting.h"
	# vfs_sub_fat
	"${CMAKE_CURRENT_SOURCE_DIR}/vfs_sub_fat_cook.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/vfs_sub_fat_cook.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/vfs_sub_fat_resource.cpp"
	# math
	# float4x4
	"${CMAKE_CURRENT_SOURCE_DIR}/math_float4x4.cpp"
	# sphere
	"${CMAKE_CURRENT_SOURCE_DIR}/math_sphere.cpp"
	# obb
	"${CMAKE_CURRENT_SOURCE_DIR}/math_obb.cpp"
	# aabb
	"${CMAKE_CURRENT_SOURCE_DIR}/math_aabb.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/math_aabb_ray_intersection.cpp"
	# cylinder
	"${CMAKE_CURRENT_SOURCE_DIR}/math_cylinder.cpp"
	# float3
	"${CMAKE_CURRENT_SOURCE_DIR}/math_float3.cpp"
	# frustum
	"${CMAKE_CURRENT_SOURCE_DIR}/math_frustum.cpp"
	# quaternion
	"${CMAKE_CURRENT_SOURCE_DIR}/math_quaternion.cpp"
	# aabb_plane
	"${CMAKE_CURRENT_SOURCE_DIR}/math_aabb_plane.cpp"
	# cuboid
	"${CMAKE_CURRENT_SOURCE_DIR}/math_cuboid.cpp"
	# constants
	"${CMAKE_CURRENT_SOURCE_DIR}/math_constants.cpp"
	# tests
	#"${CMAKE_CURRENT_SOURCE_DIR}/math_tests.cpp"
	# functions
	"${CMAKE_CURRENT_SOURCE_DIR}/math_functions.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/math_functions_win_xbox360.cpp"
	# curve
	"${CMAKE_CURRENT_SOURCE_DIR}/math_curve.cpp"
	# combinatorics
	"${CMAKE_CURRENT_SOURCE_DIR}/math_permutations.cpp"
	# convex
	"${CMAKE_CURRENT_SOURCE_DIR}/math_convex.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_convex.h"
	# build
	"${CMAKE_CURRENT_SOURCE_DIR}/build_extensions.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/build_extensions.h"
	# timing
	"${CMAKE_CURRENT_SOURCE_DIR}/timing.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/timing_functions_win.cpp"
	# configs
	# ltx
	"${CMAKE_CURRENT_SOURCE_DIR}/configs_ltx_config.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/configs_ltx_config.h"
	# cook
	"${CMAKE_CURRENT_SOURCE_DIR}/configs_ltx_config_cook.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/configs_ltx_config_cook.h"
	# binary
	"${CMAKE_CURRENT_SOURCE_DIR}/configs_binary_config.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/configs_binary_config.h"
	# value
	"${CMAKE_CURRENT_SOURCE_DIR}/configs_binary_config_value.cpp"
	# cook
	"${CMAKE_CURRENT_SOURCE_DIR}/configs_binary_config_cook.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/configs_binary_config_cook.h"
	# lua
	"${CMAKE_CURRENT_SOURCE_DIR}/configs_lua_config.cpp"
	# wrapper
	"${CMAKE_CURRENT_SOURCE_DIR}/script_engine_wrapper.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/script_engine_wrapper.h"
	# cook
	"${CMAKE_CURRENT_SOURCE_DIR}/configs_lua_config_cook.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/configs_lua_config_cook.h"
	# console_commands
	"${CMAKE_CURRENT_SOURCE_DIR}/console_command.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/console_command_processor.cpp"
	# compression
	#"${CMAKE_CURRENT_SOURCE_DIR}/compressors_test.cpp"
	# ppmd
	"${CMAKE_CURRENT_SOURCE_DIR}/compressor_ppmd.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/compressor_ppmd_allocator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/compressor_ppmd_stream.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/compressor_ppmd_stream_inline.h"
	# command_line
	"${CMAKE_CURRENT_SOURCE_DIR}/command_line.cpp"
	# testing
	"${CMAKE_CURRENT_SOURCE_DIR}/testing_impl.cpp"
	# tasks
	"${CMAKE_CURRENT_SOURCE_DIR}/tasks_allocator.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tasks_allocator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/tasks_manager.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tasks_task.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tasks_thread_pool.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tasks_thread_pool.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/tasks_thread_pool_utils.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tasks_type.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tasks_type.h"
	# profiler
	"${CMAKE_CURRENT_SOURCE_DIR}/profiler.cpp"
	# core
	"${CMAKE_CURRENT_SOURCE_DIR}/core_debug_engine.cpp"
	# include
	"${CMAKE_CURRENT_SOURCE_DIR}/../../extensions.h"
	# containers
	"${CMAKE_CURRENT_SOURCE_DIR}/../../containers_extensions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../debug_policies.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../size_policies.h"
	# concurrent
	# limited_intrusive_list
	"${CMAKE_CURRENT_SOURCE_DIR}/../../limited_intrusive_list.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../limited_intrusive_list_inline.h"
	# intrusive_mpsc_queue
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_mpsc_queue.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_mpsc_queue_inline.h"
	# intrusive_spsc_queue
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_spsc_queue.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_spsc_queue_inline.h"
	# intrusive_spmc_queue
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_spmc_queue.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_spmc_queue_inline.h"
	# intrusive_mpmc_queue
	"${CMAKE_CURRENT_SOURCE_DIR}/intrusive_mpmc_queue.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_mpmc_queue_inline.h"
	# channels
	"${CMAKE_CURRENT_SOURCE_DIR}/../../one_way_threads_channel_with_response.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../one_way_threads_channel_with_response_inline.h"
	# thread_unsafety
	# fixed_vector
	"${CMAKE_CURRENT_SOURCE_DIR}/../../fixed_vector.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../fixed_vector_inline.h"
	# hash_multiset
	"${CMAKE_CURRENT_SOURCE_DIR}/../../hash_multiset.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../hash_multiset_inline.h"
	# buffer_vector
	"${CMAKE_CURRENT_SOURCE_DIR}/../../buffer_vector.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../buffer_vector_inline.h"
	# associative_vector
	"${CMAKE_CURRENT_SOURCE_DIR}/../../associative_vector.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../associative_vector_inline.h"
	# compare_predicate
	"${CMAKE_CURRENT_SOURCE_DIR}/../../associative_vector_compare_predicate.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../associative_vector_compare_predicate_inline.h"
	# std
	"${CMAKE_CURRENT_SOURCE_DIR}/../../std_containers.h"
	# vector
	"${CMAKE_CURRENT_SOURCE_DIR}/../../vector.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../vector_inline.h"
	# set
	"${CMAKE_CURRENT_SOURCE_DIR}/../../set.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../set_inline.h"
	# map
	"${CMAKE_CURRENT_SOURCE_DIR}/../../map.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../map_inline.h"
	# allocator
	"${CMAKE_CURRENT_SOURCE_DIR}/../../std_allocator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../std_allocator_inline.h"
	# multiset
	"${CMAKE_CURRENT_SOURCE_DIR}/../../multiset.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../multiset_inline.h"
	# deque
	"${CMAKE_CURRENT_SOURCE_DIR}/../../deque.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../deque_inline.h"
	# multimap
	"${CMAKE_CURRENT_SOURCE_DIR}/../../multimap.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../multimap_inline.h"
	# vectora
	"${CMAKE_CURRENT_SOURCE_DIR}/../../vectora.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../vectora_inline.h"
	# c_array
	"${CMAKE_CURRENT_SOURCE_DIR}/../../c_array_functions.h"
	# policy
	# intrusive_double_linked_list
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_double_linked_list.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_double_linked_list_inline.h"
	# intrusive_list
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_list.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_list_inline.h"
	# intrusive_queue
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_queue.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_queue_inline.h"
	# macro
	"${CMAKE_CURRENT_SOURCE_DIR}/../../macro_extensions.h"
	# macros
	"${CMAKE_CURRENT_SOURCE_DIR}/../../macro_building.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../macro_compile_assert.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../macro_compiler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../macro_library_name.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../macro_platform.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../macro_strings.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../macro_todo.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../macro_unreferenced_parameter.h"
	# optimization
	"${CMAKE_CURRENT_SOURCE_DIR}/../../optimization_extensions.h"
	# type
	"${CMAKE_CURRENT_SOURCE_DIR}/../../type_enum_flags.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../type_extensions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../type_predicates.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../type_predicates_inline.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../type_uid_object.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../type_variant.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../type_variant_inline.h"
	# os_extensions
	"${CMAKE_CURRENT_SOURCE_DIR}/../../os_extensions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../os_include.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../os_preinclude.h"
	# functions
	"${CMAKE_CURRENT_SOURCE_DIR}/../../os_functions.h"
	# threading
	"${CMAKE_CURRENT_SOURCE_DIR}/../../threading_active_object.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../threading_extensions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../threading_reader_writer_lock.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../threading_simple_lock.h"
	# primitives
	"${CMAKE_CURRENT_SOURCE_DIR}/../../threading_event.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../threading_mutex.h"
	# functions
	"${CMAKE_CURRENT_SOURCE_DIR}/../../threading_functions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../threading_functions_guard.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../threading_functions_ps3_inline.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../threading_functions_tls.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../threading_functions_win_inline.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../threading_functions_xbox_inline.h"
	# policies
	"${CMAKE_CURRENT_SOURCE_DIR}/../../threading_policies.h"
	# one_way_channel
	"${CMAKE_CURRENT_SOURCE_DIR}/../../one_way_threads_channel.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../one_way_threads_channel_inline.h"
	# linkage
	"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../linkage_helper.h"
	# core
	"${CMAKE_CURRENT_SOURCE_DIR}/../core.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../core_debug_engine.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../exit_codes.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../simple_engine.h"
	# timing
	"${CMAKE_CURRENT_SOURCE_DIR}/../../timing_extensions.h"
	# timer
	"${CMAKE_CURRENT_SOURCE_DIR}/../../timing_timer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../timing_timer_inline.h"
	# functions
	"${CMAKE_CURRENT_SOURCE_DIR}/../../timing_functions.h"
	# math
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_constants.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_extensions.h"
	# functions
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_functions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_functions_inline.h"
	# float2
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_float2.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_float2_inline.h"
	# float3
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_float3.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_float3_inline.h"
	# float4
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_float4.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_float4_inline.h"
	# float4x4
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_float4x4.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_float4x4_inline.h"
	# plane
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_plane.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_plane_inline.h"
	# sphere
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_sphere.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_sphere_inline.h"
	# aabb
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_aabb.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_aabb_inline.h"
	# obb
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_obb.h"
	# rectangle
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_rectangle.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_rectangle_inline.h"
	# quaternion
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_quaternion.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_quaternion_inline.h"
	# cylinder
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_cylinder.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_cylinder_inline.h"
	# frustum
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_frustum.h"
	# color
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_color.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_color_inline.h"
	# aabb_plane
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_aabb_plane.h"
	# int2
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_int2.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_int2_inline.h"
	# cuboid
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_cuboid.h"
	# random
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_randoms_generator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_randoms_table.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_randoms_table_inline.h"
	# half
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_half.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_half_inline.h"
	# curve
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_curve.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_curve_inline.h"
	# combinatorics
	"${CMAKE_CURRENT_SOURCE_DIR}/../../math_permutations.h"
	# resources
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_callbacks.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_classes.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_cook_base.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_cook_classes.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_fs.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_fs_iterator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_memory.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_memory_usage.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_mount_ptr.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_reader.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../unmanaged_allocation_resource.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../vfs_sub_fat_resource.h"
	# resource
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_intrusive_base.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_managed_resource.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_name_registry_entry.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_pinned_ptr.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_resource.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_resource_base.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_resource_children.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_resource_flags.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_resource_link.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_resource_ptr.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_resource_ptr_inline.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_resource_quality.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_resource_reconstruction_info.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_types.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_unmanaged_resource.h"
	# query_result
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_queries_result.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_query_result.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../resources_save_generated_data.h"
	# fs
	"${CMAKE_CURRENT_SOURCE_DIR}/../../fs_platform_configuration.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../fs_sub_fat_resource.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../fs_utils.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../fs_watcher.h"
	# path_string
	"${CMAKE_CURRENT_SOURCE_DIR}/../../fs_path.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../fs_path_string.h"
	# path_iterator
	"${CMAKE_CURRENT_SOURCE_DIR}/../../fs_path_iterator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../fs_path_iterator_inline.h"
	# pointers
	"${CMAKE_CURRENT_SOURCE_DIR}/../../pointer_extensions.h"
	# intrusive
	# base
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_base.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_base_inline.h"
	# pointer
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_ptr.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../intrusive_ptr_inline.h"
	# debug
	"${CMAKE_CURRENT_SOURCE_DIR}/../../debug/debug_pointers.h"
	# non_null
	"${CMAKE_CURRENT_SOURCE_DIR}/../../non_null_ptr.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../non_null_ptr_inline.h"
	# loose
	# pointer
	"${CMAKE_CURRENT_SOURCE_DIR}/../../loose_ptr.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../loose_ptr_inline.h"
	# base
	"${CMAKE_CURRENT_SOURCE_DIR}/../../loose_ptr_base.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../loose_ptr_base_inline.h"
	# data
	"${CMAKE_CURRENT_SOURCE_DIR}/../../loose_ptr_data.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../loose_ptr_data_inline.h"
	# raw
	"${CMAKE_CURRENT_SOURCE_DIR}/../../raw_ptr.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../raw_ptr_inline.h"
	# platform
	"${CMAKE_CURRENT_SOURCE_DIR}/../../platform_pointer.h"
	# unique
	"${CMAKE_CURRENT_SOURCE_DIR}/../../unique_ptr.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../unique_ptr_inline.h"
	# console_commands
	"${CMAKE_CURRENT_SOURCE_DIR}/../../console_command.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../console_command_inline.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../console_command_processor.h"
	# strings
	"${CMAKE_CURRENT_SOURCE_DIR}/../../strings_extensions.h"
	# shared
	# string
	"${CMAKE_CURRENT_SOURCE_DIR}/../../shared_string.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../shared_string_inline.h"
	# profile
	"${CMAKE_CURRENT_SOURCE_DIR}/../../strings_shared_profile.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../strings_shared_profile_inline.h"
	# functions
	"${CMAKE_CURRENT_SOURCE_DIR}/../../strings_functions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../strings_functions_inline.h"
	# concatenations
	"${CMAKE_CURRENT_SOURCE_DIR}/../../strings_concatenations.h"
	# detail
	"${CMAKE_CURRENT_SOURCE_DIR}/../../strings_detail_tuples.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../strings_detail_tuples_inline.h"
	# fixed
	"${CMAKE_CURRENT_SOURCE_DIR}/../../fixed_string.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../fixed_string_inline.h"
	# buffer
	"${CMAKE_CURRENT_SOURCE_DIR}/../../buffer_string.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../buffer_string_inline.h"
	# stream
	"${CMAKE_CURRENT_SOURCE_DIR}/../../strings_stream.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../strings_stream_inline.h"
	# text_tree
	"${CMAKE_CURRENT_SOURCE_DIR}/../../text_tree.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../text_tree_item.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../text_tree_log_helper.h"
	# configs
	"${CMAKE_CURRENT_SOURCE_DIR}/../../configs.h"
	# binary
	"${CMAKE_CURRENT_SOURCE_DIR}/../../configs_binary_config.h"
	# value
	"${CMAKE_CURRENT_SOURCE_DIR}/../../configs_binary_config_value.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../configs_binary_config_value_inline.h"
	# detail
	"${CMAKE_CURRENT_SOURCE_DIR}/../../detail_noncopyable.h"
	# lua
	"${CMAKE_CURRENT_SOURCE_DIR}/../../configs_lua_config.h"
	# value
	"${CMAKE_CURRENT_SOURCE_DIR}/../../configs_lua_config_value.h"
	# command_line
	"${CMAKE_CURRENT_SOURCE_DIR}/../../command_line_extensions.h"
	# compression
	"${CMAKE_CURRENT_SOURCE_DIR}/../../compressor.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../compressor_ppmd.h"
	# platform
	"${CMAKE_CURRENT_SOURCE_DIR}/../../platform_extensions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../platform_extensions_win.h"
	# testing
	"${CMAKE_CURRENT_SOURCE_DIR}/../../testing.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../testing_inline.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../testing_macro.h"
	# suites
	"${CMAKE_CURRENT_SOURCE_DIR}/../../core_test_suite.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../engine_test_suite.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../game_test_suite.h"
	# memory
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_extensions.h"
	# allocators
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_synchronized_allocator.h"
	# wrappers
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_allocator_helper.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_allocator_helper_inline.h"
	# dlmalloc
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_doug_lea_allocator.h"
	# ptmalloc3
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_pthreads3_allocator.h"
	# base
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_base_allocator.h"
	# crt
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_crt_allocator.h"
	# dlmalloc_mt
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_doug_lea_mt_allocator.h"
	# stack
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_stack_allocator.h"
	# fixed_size
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_fixed_size_allocator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_fixed_size_allocator_inline.h"
	# debug
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_debug_allocator.h"
	# single_size_buffer
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_single_size_buffer_allocator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_single_size_buffer_allocator_inline.h"
	# policies
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_multi_threading_single_size_allocator_policy.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_multi_threading_single_size_allocator_policy_inline.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_single_threading_single_size_allocator_policy.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_single_threading_single_size_allocator_policy_inline.h"
	# single_size_fixed
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_single_size_fixed_allocator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_single_size_fixed_allocator_inline.h"
	# managed
	"${CMAKE_CURRENT_SOURCE_DIR}/../../managed_allocator.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../managed_node_owner.h"
	# impl
	"${CMAKE_CURRENT_SOURCE_DIR}/../../managed_allocator_base.h"
	# debug
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_debug.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_debug_parameters.h"
	# macros
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_macros.h"
	# reader
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_reader.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_reader_inline.h"
	# wrapper
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_reader_wrapper.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_reader_wrapper_inline.h"
	# chunk
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_chunk_reader.h"
	# sequential
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_sequential_chunk_reader.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_sequential_chunk_reader_inline.h"
	# associative
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_associative_chunk_reader.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_associative_chunk_reader_inline.h"
	# array
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_array_chunk_reader.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_array_chunk_reader_inline.h"
	# abstract
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_reader_abstract.h"
	# stream
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_stream.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_stream_inline.h"
	# buffer
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_allocated_buffer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_buffer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_buffer_inline.h"
	# operators
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_override_operators.h"
	# uninitialized_reference
	"${CMAKE_CURRENT_SOURCE_DIR}/../../construction.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../uninitialized_reference.h"
	# writer
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_writer.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../memory_writer_inline.h"
	# tasks
	"${CMAKE_CURRENT_SOURCE_DIR}/../../tasks_system.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../tasks_task.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../tasks_thread_tls.h"
	# geometry_utils
	"${CMAKE_CURRENT_SOURCE_DIR}/geometry_primitives.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../geometry_primitives.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/geometry_utils.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../geometry_utils.h"
	# stdlib
	"${CMAKE_CURRENT_SOURCE_DIR}/../../stdlib_extensions.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../stdlib_extensions_inline.h"
	# profiler
	"${CMAKE_CURRENT_SOURCE_DIR}/../../profiler.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../profiler_inline.h"
	# ./
	"${CMAKE_CURRENT_SOURCE_DIR}/core_entry_point.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/../../core_entry_point.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core_entry_point_win.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/core_entry_point_win_xbox360.cpp"
)

target_link_libraries(core PUBLIC
	debugging
	logging
)

set_target_properties(core PROPERTIES FOLDER "engine/core")
