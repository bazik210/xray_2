project(rtp LANGUAGES CXX)

add_library(rtp STATIC
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/rtp_library_linkage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/rtp_memory.h"
					# world
					"${CMAKE_CURRENT_SOURCE_DIR}/rtp_world.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/rtp_world.h"
					# include
					"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../base_controller.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
					# action
					"${CMAKE_CURRENT_SOURCE_DIR}/action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/action.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/action_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_action.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_action_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_action_params.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/dbg_sample_check_res.h"
					# controller
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_controller.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_controller_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/controller.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/controller.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/controller_inline.h"
					# state
					"${CMAKE_CURRENT_SOURCE_DIR}/state.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/state.h"
					# space_params
					"${CMAKE_CURRENT_SOURCE_DIR}/space_params.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/space_params.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/space_params_inline.h"
					# test
					# grasping
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_grasping_action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_grasping_action.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_grasping_controller.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_grasping_controller.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_grasping_params.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_grasping_params.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_grasping_params_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/grasping_space_params.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/grasping_space_params.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/test_grasping_action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/test_grasping_action.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/test_grasping_controller.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/test_grasping_controller.h"
					# navigation
					"${CMAKE_CURRENT_SOURCE_DIR}/navigation_action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/navigation_action.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/navigation_action_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/navigation_controller.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/navigation_controller.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/navigation_params.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/navigation_params.h"
					# value
					# tree
					"${CMAKE_CURRENT_SOURCE_DIR}/regression_tree.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/regression_tree.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/regression_tree_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/training_regression_tree.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/training_regression_tree.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/training_regression_tree_inline.h"
					# value_function
					"${CMAKE_CURRENT_SOURCE_DIR}/action_value.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/action_value_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/training_value_function.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/training_value_function.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/training_value_function_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/value_function.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/value_function.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/value_function_inline.h"
					# sets
					"${CMAKE_CURRENT_SOURCE_DIR}/training_set.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/training_set.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/training_set_inline.h"
					# trajectory
					"${CMAKE_CURRENT_SOURCE_DIR}/trajectory.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/trajectory.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/trajectory_inline.h"
					# action_base
					"${CMAKE_CURRENT_SOURCE_DIR}/action_base.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/action_base.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/action_base_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/iaction.h"
					# testing
					"${CMAKE_CURRENT_SOURCE_DIR}/testing.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/testing.h"
					# resources
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_cooks.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_cooks.h"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/rtp_entry_point.cpp"
)

target_link_libraries(rtp PRIVATE
	engine_pc
)

set_target_properties(rtp PROPERTIES FOLDER "engine/animation")
