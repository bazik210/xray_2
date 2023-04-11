project(animation LANGUAGES CXX)

add_library(animation STATIC
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/animation_library_linkage.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/animation_memory.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						# include
						"${CMAKE_CURRENT_SOURCE_DIR}/../animation.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../animation_containers.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../animation_expression_emitter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../noncopyable.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../type_definitions.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
						# animation_data_interface
						"${CMAKE_CURRENT_SOURCE_DIR}/../anim_track_common.h"
						# controller
						"${CMAKE_CURRENT_SOURCE_DIR}/../i_animation_action.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../i_animation_controller.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../i_animation_controller_set.h"
						# editor
						"${CMAKE_CURRENT_SOURCE_DIR}/../i_editor_animation.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../i_editor_mixer.h"
						# camera_follower
						"${CMAKE_CURRENT_SOURCE_DIR}/../camera_follower.h"
						# track
						"${CMAKE_CURRENT_SOURCE_DIR}/../anim_track.h"
						# resources
						# animation_player
						"${CMAKE_CURRENT_SOURCE_DIR}/../animation_player.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../animation_player_inline.h"
						# animation_interval
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_animation_interval.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_animation_interval_inline.h"
						# binary_tree
						# nodes
						# addition
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_addition_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_addition_node_inline.h"
						# animation
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_animation_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_animation_node_inline.h"
						# base
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_base_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_base_node_inline.h"
						# binary_operation
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_binary_operation_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_binary_operation_node_inline.h"
						# multiplication
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_multiplication_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_multiplication_node_inline.h"
						# subtraction
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_subtraction_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_subtraction_node_inline.h"
						# weight
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_weight_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_weight_node_inline.h"
						# operations
						# visitor
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_tree_visitor.h"
						# interpolators
						# base
						"${CMAKE_CURRENT_SOURCE_DIR}/../base_interpolator.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../base_interpolator_inline.h"
						# linear
						"${CMAKE_CURRENT_SOURCE_DIR}/../linear_interpolator.h"
						# fermi
						"${CMAKE_CURRENT_SOURCE_DIR}/../fermi_interpolator.h"
						# instant
						"${CMAKE_CURRENT_SOURCE_DIR}/../instant_interpolator.h"
						# operations
						"${CMAKE_CURRENT_SOURCE_DIR}/../interpolator_visitor.h"
						# lexems
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_debug.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_math.h"
						# animation
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_animation_lexeme.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_animation_lexeme_inline.h"
						# parameters
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_animation_lexeme_parameters.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_animation_lexeme_parameters_inline.h"
						# weight
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_weight_lexeme.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_weight_lexeme_inline.h"
						# multiplication
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_multiplication_lexeme.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_multiplication_lexeme_inline.h"
						# base
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_base_lexeme.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_base_lexeme_inline.h"
						# expression
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_expression.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_expression_inline.h"
						# addition
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_addition_lexeme.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_addition_lexeme_inline.h"
						# subtraction
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_subtraction_lexeme.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_subtraction_lexeme_inline.h"
						# binary_operation
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_operation_lexeme.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_binary_operation_lexeme_inline.h"
						# animation_callback
						"${CMAKE_CURRENT_SOURCE_DIR}/../animation_callback.h"
						# cubic_spline_skeleton_animation
						"${CMAKE_CURRENT_SOURCE_DIR}/../cubic_spline_skeleton_animation.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../cubic_spline_skeleton_animation_inline.h"
						# bone_animation
						"${CMAKE_CURRENT_SOURCE_DIR}/../bone_animation.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../bone_animation_inline.h"
						# poly_curve
						"${CMAKE_CURRENT_SOURCE_DIR}/../poly_curve.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../poly_curve_inline.h"
						# animation_event_channels
						"${CMAKE_CURRENT_SOURCE_DIR}/../animation_event_channels.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../animation_event_channels_inline.h"
						# event_channel
						"${CMAKE_CURRENT_SOURCE_DIR}/../event_channel.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../event_channel_inline.h"
						# time_channel
						"${CMAKE_CURRENT_SOURCE_DIR}/../time_channel.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../time_channel_inline.h"
						# bone_names
						"${CMAKE_CURRENT_SOURCE_DIR}/../bone_names.h"
						# editable_event_channels
						"${CMAKE_CURRENT_SOURCE_DIR}/../editable_animation_event_channels.h"
						# skeleton
						"${CMAKE_CURRENT_SOURCE_DIR}/../skeleton.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../skeleton_inline.h"
						# bone
						"${CMAKE_CURRENT_SOURCE_DIR}/../skeleton_bone.h"
						# skeleton_animation
						"${CMAKE_CURRENT_SOURCE_DIR}/../skeleton_animation.h"
						# bi_spline_skeleton_animation
						# bone_animation
						# data
						"${CMAKE_CURRENT_SOURCE_DIR}/../bi_spline_data.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../bi_spline_data_inline.h"
						# world
						"${CMAKE_CURRENT_SOURCE_DIR}/animation_world.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/animation_world.h"
						# resources
						# skeleton
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton.cpp"
						# cook
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_cook.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_cook.h"
						# bone
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_bone.cpp"
						# skeleton_animation
						# cook
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_animation_cook.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_animation_cook.h"
						# bi_spline_skeleton_animation
						"${CMAKE_CURRENT_SOURCE_DIR}/bi_spline_skeleton_animation.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/bi_spline_skeleton_animation.h"
						# check
						"${CMAKE_CURRENT_SOURCE_DIR}/check_animation_data.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/check_animation_data.h"
						# bone_animation
						"${CMAKE_CURRENT_SOURCE_DIR}/bi_spline_bone_animation.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/bi_spline_bone_animation.h"
						# data
						# cubic_spline_skeleton_animation
						"${CMAKE_CURRENT_SOURCE_DIR}/cubic_spline_skeleton_animation.cpp"
						# bone_animation
						"${CMAKE_CURRENT_SOURCE_DIR}/bone_animation.cpp"
						# poly_curve
						# editable_event_channels
						"${CMAKE_CURRENT_SOURCE_DIR}/editable_animation_event_channels.cpp"
						# bone_names
						"${CMAKE_CURRENT_SOURCE_DIR}/bone_names.cpp"
						# animation_event_channels
						"${CMAKE_CURRENT_SOURCE_DIR}/animation_event_channels.cpp"
						# event_channel
						"${CMAKE_CURRENT_SOURCE_DIR}/event_channel.cpp"
						# time_channel
						# cook
						"${CMAKE_CURRENT_SOURCE_DIR}/cubic_spline_skeleton_animation_cook.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/cubic_spline_skeleton_animation_cook.h"
						# animation_player
						"${CMAKE_CURRENT_SOURCE_DIR}/animation_player.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/animation_player_test.cpp"
						# trees
						# binary tree
						# nodes
						# multiplication
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_binary_tree_multiplication_node.cpp"
						# addition
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_binary_tree_addition_node.cpp"
						# animation
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_binary_tree_animation_node.cpp"
						# subtraction
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_binary_tree_subtraction_node.cpp"
						# weight
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_binary_tree_weight_node.cpp"
						# base
						# binary_operation
						# operations
						# node_constructor
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_node_constructor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_node_constructor.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_node_constructor_inline.h"
						# weaver
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weaver.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weaver.h"
						# redundant_multiplicands_detector
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_redundant_multiplicands_detector.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_redundant_multiplicands_detector.h"
						# null_weight_searcher
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_binary_tree_null_weight_searcher.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_binary_tree_null_weight_searcher.h"
						# expression_simplifier
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_binary_tree_expression_simplifier.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_binary_tree_expression_simplifier.h"
						# writer
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_binary_tree_writer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_binary_tree_writer.h"
						# lexems
						# animation
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_animation_lexeme.cpp"
						# parameters
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_animation_lexeme_parameters.cpp"
						# n-ary tree
						# nodes
						# multiplication
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_multiplication_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_multiplication_node.h"
						# addition
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_addition_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_addition_node.h"
						# subtraction
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_subtraction_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_subtraction_node.h"
						# base
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_base_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_base_node_inline.h"
						# weight
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weight_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weight_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weight_node_inline.h"
						# animation
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_animation_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_animation_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_animation_node_inline.h"
						# n_ary_operation
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_n_ary_operation_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_n_ary_operation_node_inline.h"
						# time_scale
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_time_scale_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_time_scale_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_time_scale_node_inline.h"
						# transitions
						# weight
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weight_transition_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weight_transition_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weight_transition_node_inline.h"
						# time_scale
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_time_scale_transition_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_time_scale_transition_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_time_scale_transition_node_inline.h"
						# operations
						# comparer
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_comparer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_comparer.h"
						# destroyer
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_destroyer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_destroyer.h"
						# double_dispatcher
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_double_dispatcher.h"
						# visitor
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_visitor.h"
						# interpolator_selector
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_interpolator_selector.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_interpolator_selector.h"
						# node_comparer
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_node_comparer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_node_comparer.h"
						# size_calculator
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_size_calculator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_size_calculator.h"
						# transition_tree_constructor
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_transition_tree_constructor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_transition_tree_constructor.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_transition_tree_constructor_impl.h"
						# cloner
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_cloner.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_cloner.h"
						# dumper
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_dumper.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_dumper.h"
						# time_scale_calculator
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_time_scale_calculator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_time_scale_calculator.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_time_scale_calculator_inline.h"
						# transition_to_one_detector
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_transition_to_one_detector.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_transition_to_one_detector.h"
						# target_time_scale_calculator
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_target_time_scale_calculator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_target_time_scale_calculator.h"
						# animation_time_calculator
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_animation_time_calculator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_animation_time_calculator.h"
						# time_scale_start_time_modifier
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_time_scale_start_time_modifier.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_time_scale_start_time_modifier.h"
						# weight_calculator
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weight_calculator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weight_calculator.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weight_calculator_inline.h"
						# animation_state
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_animation_state.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_animation_state.h"
						# event_iterator
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_event_iterator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_event_iterator.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_event_iterator_inline.h"
						# animation_event_iterator
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_animation_event_iterator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_animation_event_iterator.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_animation_event_iterator_inline.h"
						# time_in_ms_calculator
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_time_in_ms_calculator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_time_in_ms_calculator.h"
						# weight_event_iterator
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weight_event_iterator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weight_event_iterator.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_weight_event_iterator_inline.h"
						# animation_event
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_animation_event.h"
						# tree
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_n_ary_tree.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_n_ary_tree_inline.h"
						# intrusive_base
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_n_ary_tree_intrusive_base.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../mixing_n_ary_tree_intrusive_base_inline.h"
						# bone_matrices_computer
						"${CMAKE_CURRENT_SOURCE_DIR}/bone_matrices_computer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/bone_matrices_computer.h"
						# binary to n-ary tree converter
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_converter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/mixing_n_ary_tree_converter.h"
						# interpolators
						# interpolators
						# linear
						"${CMAKE_CURRENT_SOURCE_DIR}/linear_interpolator.cpp"
						# base
						"${CMAKE_CURRENT_SOURCE_DIR}/base_interpolator.cpp"
						# fermi
						"${CMAKE_CURRENT_SOURCE_DIR}/fermi_interpolator.cpp"
						# instant
						"${CMAKE_CURRENT_SOURCE_DIR}/instant_interpolator.cpp"
						# operations
						# size_calculator
						"${CMAKE_CURRENT_SOURCE_DIR}/interpolator_size_calculator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/interpolator_size_calculator.h"
						# comparer
						"${CMAKE_CURRENT_SOURCE_DIR}/interpolator_comparer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/interpolator_comparer.h"
						# maya_animation_track
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_track.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_eval.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_io.cpp"
						# editor
						# animation
						"${CMAKE_CURRENT_SOURCE_DIR}/editor_animation.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/editor_animation.h"
						# editor_mixer
						"${CMAKE_CURRENT_SOURCE_DIR}/editor_mixer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/editor_mixer.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/i_editor_mixer_inline.h"
						# camera_follower
						"${CMAKE_CURRENT_SOURCE_DIR}/camera_follower.cpp"
						# objects
						# collection
						"${CMAKE_CURRENT_SOURCE_DIR}/animation_collection.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/animation_collection.h"
						# cook
						"${CMAKE_CURRENT_SOURCE_DIR}/animation_collection_cook.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/animation_collection_cook.h"
						# single
						"${CMAKE_CURRENT_SOURCE_DIR}/single_animation.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/single_animation.h"
						# cook
						"${CMAKE_CURRENT_SOURCE_DIR}/single_animation_cook.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/single_animation_cook.h"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/animation_entry_point.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/roadmap.cpp"
)

set_target_properties(animation PROPERTIES FOLDER "engine/animation")
