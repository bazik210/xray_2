project(graphics_benchmark LANGUAGES CXX)

add_executable(graphics_benchmark
						# application
						"${CMAKE_CURRENT_SOURCE_DIR}/graphics_benchmark_application.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/graphics_benchmark_application.h"
						# benchmark_test
						"${CMAKE_CURRENT_SOURCE_DIR}/benchmark_test.h"
						# benchmark
						"${CMAKE_CURRENT_SOURCE_DIR}/benchmark.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/benchmark.h"
						# helpers
						"${CMAKE_CURRENT_SOURCE_DIR}/benchmark_helpers.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/benchmark_helpers.h"
						# types
						"${CMAKE_CURRENT_SOURCE_DIR}/benchmark_types.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/counters_result.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/counters_result.h"
						# sphere_primitive
						"${CMAKE_CURRENT_SOURCE_DIR}/sphere_primitive.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/sphere_primitive.h"
						# timers
						# gpu_timer
						"${CMAKE_CURRENT_SOURCE_DIR}/gpu_timer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/gpu_timer.h"
						# gpu_counter
						"${CMAKE_CURRENT_SOURCE_DIR}/gpu_counter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/gpu_counter.h"
						# benchmark_renderer
						"${CMAKE_CURRENT_SOURCE_DIR}/benchmark_renderer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/benchmark_renderer.h"
						# tests
						# test_simple_shader_op
						"${CMAKE_CURRENT_SOURCE_DIR}/test_simple_shader_op.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/test_simple_shader_op.h"
						# test_clear_render_target_depth_stencil
						"${CMAKE_CURRENT_SOURCE_DIR}/test_clear_render_target_depth_stencil.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/test_clear_render_target_depth_stencil.h"
						# test_2d_shader_op
						"${CMAKE_CURRENT_SOURCE_DIR}/test_2d_shader_op.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/test_2d_shader_op.h"
						# test_3d_shader_op
						"${CMAKE_CURRENT_SOURCE_DIR}/test_3d_shader_op.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/test_3d_shader_op.h"
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/graphics_benchmark_library_linkage.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/graphics_benchmark_memory.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/math_error.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						# resources
						"${CMAKE_CURRENT_SOURCE_DIR}/graphics_benchmark.ico"
						"${CMAKE_CURRENT_SOURCE_DIR}/graphics_benchmark.rc"
						"${CMAKE_CURRENT_SOURCE_DIR}/graphics_benchmark_resources.h"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/graphics_benchmark_entry_point.cpp"
)

set_target_properties(graphics_benchmark PROPERTIES FOLDER "engine/utilities")
