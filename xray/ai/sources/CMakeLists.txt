project(ai LANGUAGES CXX)

add_library(ai STATIC
                # core
                "${CMAKE_CURRENT_SOURCE_DIR}/ai_library_linkage.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/ai_memory.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
                # world
                "${CMAKE_CURRENT_SOURCE_DIR}/ai_world.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/ai_world.h"
                # subscribers
                "${CMAKE_CURRENT_SOURCE_DIR}/damage_subscriber.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/game_object_subscriber.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/sensors_subscriber.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/sound_subscriber.h"
                # include
                "${CMAKE_CURRENT_SOURCE_DIR}/../action_types.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../animation_item.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../collision_object.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../filter_types.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../game_object.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../goal_types.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../movement_target.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../navigation_environment.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../npc.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../npc_statistics.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../parameter_types.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../sensed_hit_object.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../sensed_sound_object.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../sensed_visual_object.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../sound_collection_types.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../sound_item.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../sound_player.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../weapon.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
                # brain_unit
                "${CMAKE_CURRENT_SOURCE_DIR}/brain_unit.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/brain_unit.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/brain_unit_perceptors.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/brain_unit_selectors.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/brain_unit_sensors.cpp"
                # blackboard
                "${CMAKE_CURRENT_SOURCE_DIR}/blackboard.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/blackboard.h"
                # perceptors
                # base
                "${CMAKE_CURRENT_SOURCE_DIR}/perceptor_base.h"
                # enemy
                "${CMAKE_CURRENT_SOURCE_DIR}/enemy_perceptor.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/enemy_perceptor.h"
                # pickup_item
                "${CMAKE_CURRENT_SOURCE_DIR}/pickup_item_perceptor.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/pickup_item_perceptor.h"
                # sensors
                "${CMAKE_CURRENT_SOURCE_DIR}/sensed_object.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/sensed_object_types.h"
                # active
                "${CMAKE_CURRENT_SOURCE_DIR}/active_sensor_base.h"
                # interaction
                "${CMAKE_CURRENT_SOURCE_DIR}/interaction_sensor.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/interaction_sensor.h"
                # vision
                "${CMAKE_CURRENT_SOURCE_DIR}/vision_sensor.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/vision_sensor.h"
                # passive
                "${CMAKE_CURRENT_SOURCE_DIR}/passive_sensor_base.h"
                # damage
                "${CMAKE_CURRENT_SOURCE_DIR}/damage_sensor.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/damage_sensor.h"
                # hearing
                "${CMAKE_CURRENT_SOURCE_DIR}/hearing_sensor.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/hearing_sensor.h"
                # smell
                "${CMAKE_CURRENT_SOURCE_DIR}/smell_sensor.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/smell_sensor.h"
                # filter
                "${CMAKE_CURRENT_SOURCE_DIR}/pre_perceptors_filter.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/pre_perceptors_filter.h"
                # parameters
                "${CMAKE_CURRENT_SOURCE_DIR}/damage_sensor_parameters.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/damage_sensor_parameters.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/hearing_sensor_parameters.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/hearing_sensor_parameters.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/interaction_sensor_parameters.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/interaction_sensor_parameters.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/smell_sensor_parameters.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/smell_sensor_parameters.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/vision_sensor_parameters.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/vision_sensor_parameters.h"
                # target_selectors
                # base
                "${CMAKE_CURRENT_SOURCE_DIR}/target_selector_base.h"
                # enemy
                "${CMAKE_CURRENT_SOURCE_DIR}/enemy_target_selector.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/enemy_target_selector.h"
                # disturbance
                "${CMAKE_CURRENT_SOURCE_DIR}/disturbance_target_selector.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/disturbance_target_selector.h"
                # threat
                "${CMAKE_CURRENT_SOURCE_DIR}/threat_target_selector.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/threat_target_selector.h"
                # pickup_item
                "${CMAKE_CURRENT_SOURCE_DIR}/pickup_item_target_selector.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/pickup_item_target_selector.h"
                # weapon
                "${CMAKE_CURRENT_SOURCE_DIR}/weapon_target_selector.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/weapon_target_selector.h"
                # animation
                "${CMAKE_CURRENT_SOURCE_DIR}/animation_target_selector.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/animation_target_selector.h"
                # sound
                "${CMAKE_CURRENT_SOURCE_DIR}/sound_target_selector.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/sound_target_selector.h"
                # position
                "${CMAKE_CURRENT_SOURCE_DIR}/position_target_selector.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/position_target_selector.h"
                # working_memory
                "${CMAKE_CURRENT_SOURCE_DIR}/working_memory.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/working_memory.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/working_memory_inline.h"
                # percept_memory_object
                "${CMAKE_CURRENT_SOURCE_DIR}/percept_memory_object.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/percept_memory_object_types.h"
                # goal_selector
                "${CMAKE_CURRENT_SOURCE_DIR}/goal_selector.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/goal_selector.h"
                # goal_specificator
                "${CMAKE_CURRENT_SOURCE_DIR}/goal_specificator.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/goal_specificator.h"
                # goal
                "${CMAKE_CURRENT_SOURCE_DIR}/goal.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/goal.h"
                # cook
                "${CMAKE_CURRENT_SOURCE_DIR}/brain_unit_cook.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/brain_unit_cook.h"
                # behaviour
                "${CMAKE_CURRENT_SOURCE_DIR}/behaviour.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/behaviour.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/behaviour_domain.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/behaviour_filters.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/behaviour_goals.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/behaviour_problem.cpp"
                # cook
                "${CMAKE_CURRENT_SOURCE_DIR}/behaviour_cook.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/behaviour_cook.h"
                # search
                # vertex_allocators
                # fixed_count
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/vertex_allocator_fixed_count.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/vertex_allocator_fixed_count_inline.h"
                # vertex_managers
                # fixed_count_hash
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/vertex_manager_fixed_count_hash.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/vertex_manager_fixed_count_hash_inline.h"
                # priority_queues
                # binary_heap
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/priority_queue_binary_heap.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/priority_queue_binary_heap_inline.h"
                # algorithms
                # a_star
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/a_star.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/a_star_inline.h"
                # dijkstra
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/dijkstra.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/dijkstra_inline.h"
                # path_constructors
                # edge
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_constructor_edge.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_constructor_edge_inline.h"
                # base
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_constructor_base.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_constructor_base_inline.h"
                # vertex
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_constructor_vertex.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_constructor_vertex_inline.h"
                # base
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/base_types.h"
                # search_restrictors
                # generic
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_restrictor_generic.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_restrictor_generic_inline.h"
                # base
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_restrictor_base.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_restrictor_base_inline.h"
                # planning
                # propositional_planner
                "${CMAKE_CURRENT_SOURCE_DIR}/propositional_planner.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/propositional_planner.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/propositional_planner_inline.h"
                # base
                "${CMAKE_CURRENT_SOURCE_DIR}/propositional_planner_base.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/propositional_planner_base.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/propositional_planner_base_inline.h"
                # operator_holder
                "${CMAKE_CURRENT_SOURCE_DIR}/operator_holder.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/operator_holder.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/operator_holder_inline.h"
                # operator_pair
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/operator_pair.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/operator_pair_inline.h"
                # operators
                # operator
                "${CMAKE_CURRENT_SOURCE_DIR}/operator.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/operator.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/operator_inline.h"
                # base
                "${CMAKE_CURRENT_SOURCE_DIR}/operator_base.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/operator_base.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/operator_base_inline.h"
                # plan_tracker
                "${CMAKE_CURRENT_SOURCE_DIR}/plan_tracker.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/plan_tracker.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/plan_tracker_inline.h"
                # search
                "${CMAKE_CURRENT_SOURCE_DIR}/search.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_inline.h"
                # backward
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_backward.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_backward_inline.h"
                # graph_wrapper
                "${CMAKE_CURRENT_SOURCE_DIR}/graph_wrapper_planner_backward.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/graph_wrapper_planner_backward.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/graph_wrapper_planner_backward_inline.h"
                # path_constructor
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_constructor_planner_backward.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_constructor_planner_backward_inline.h"
                # path_heuristics
                "${CMAKE_CURRENT_SOURCE_DIR}/path_heuristics_planner_backward.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_heuristics_planner_backward.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_heuristics_planner_backward_inline.h"
                # search_restrictor
                "${CMAKE_CURRENT_SOURCE_DIR}/search_restrictor_planner_backward.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_restrictor_planner_backward.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_restrictor_planner_backward_inline.h"
                # base
                "${CMAKE_CURRENT_SOURCE_DIR}/search_base.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_base.h"
                # graph_wrapper
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/graph_wrapper_planner_base.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/graph_wrapper_planner_base_inline.h"
                # path_constructor
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_constructor_planner_base.h"
                # path_heuristics
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_heuristrics_planner_base.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_heuristrics_planner_base_inline.h"
                # search_restrictor
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_restrictor_planner_base.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_restrictor_planner_base_inline.h"
                # forward
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_forward.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_forward_inline.h"
                # graph_wrapper
                "${CMAKE_CURRENT_SOURCE_DIR}/graph_wrapper_planner_forward.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/graph_wrapper_planner_forward.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/graph_wrapper_planner_forward_inline.h"
                # path_constructor
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_constructor_planner_forward.h"
                # path_heuristics
                "${CMAKE_CURRENT_SOURCE_DIR}/path_heuristics_planner_forward.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_heuristics_planner_forward.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/path_heuristics_planner_forward_inline.h"
                # search_restrictor
                "${CMAKE_CURRENT_SOURCE_DIR}/search_restrictor_planner_forward.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_restrictor_planner_forward.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/search_restrictor_planner_forward_inline.h"
                # world_state
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/world_state.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/world_state_inline.h"
                # property
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/world_state_property.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/world_state_property_inline.h"
                # dnf_world_state
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/dnf_world_state.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/dnf_world_state_inline.h"
                # oracles
                # oracle_holder
                "${CMAKE_CURRENT_SOURCE_DIR}/oracle_holder.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/oracle_holder.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/oracle_holder_inline.h"
                # oracle
                "${CMAKE_CURRENT_SOURCE_DIR}/oracle.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/oracle.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/oracle_inline.h"
                # const
                "${CMAKE_CURRENT_SOURCE_DIR}/const_oracle.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/const_oracle.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/const_oracle_inline.h"
                # pddl_planner
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_planner.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_planner.h"
                # domain
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_domain.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_domain.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_domain_inline.h"
                # actions
                # generalized
                "${CMAKE_CURRENT_SOURCE_DIR}/generalized_action.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/generalized_action.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/generalized_action_inline.h"
                # specified
                "${CMAKE_CURRENT_SOURCE_DIR}/specified_action.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/specified_action.h"
                # instance
                "${CMAKE_CURRENT_SOURCE_DIR}/action_instance.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/action_instance.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/action_instance_inline.h"
                # predicates
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_predicate.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_predicate_inline.h"
                # predicate_redirector
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_predicate_redirector.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_predicate_redirector.h"
                # parameters
                "${CMAKE_CURRENT_SOURCE_DIR}/action_parameter.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/action_parameter.h"
                # filters
                # base
                "${CMAKE_CURRENT_SOURCE_DIR}/base_filter.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/base_filter.h"
                # enemy
                "${CMAKE_CURRENT_SOURCE_DIR}/enemy_filter.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/enemy_filter.h"
                # weapon
                "${CMAKE_CURRENT_SOURCE_DIR}/weapon_filter.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/weapon_filter.h"
                # cover
                "${CMAKE_CURRENT_SOURCE_DIR}/cover_filter.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/cover_filter.h"
                # animation
                "${CMAKE_CURRENT_SOURCE_DIR}/animation_filter.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/animation_filter.h"
                # sound
                "${CMAKE_CURRENT_SOURCE_DIR}/sound_filter.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/sound_filter.h"
                # position
                "${CMAKE_CURRENT_SOURCE_DIR}/position_filter.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/position_filter.h"
                # animation_item
                "${CMAKE_CURRENT_SOURCE_DIR}/animation_item.cpp"
                # sound_item
                "${CMAKE_CURRENT_SOURCE_DIR}/sound_item.cpp"
                # movement_target
                "${CMAKE_CURRENT_SOURCE_DIR}/movement_target.cpp"
                # problem
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_problem.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_problem.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_problem_inline.h"
                # object_instance
                "${CMAKE_CURRENT_SOURCE_DIR}/object_instance.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/object_instance_inline.h"
                # specified
                "${CMAKE_CURRENT_SOURCE_DIR}/specified_problem.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/specified_problem.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/specified_problem_inline.h"
                # world_state
                # property
                "${CMAKE_CURRENT_SOURCE_DIR}/predicate_types.h"
                # impl
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_world_state_property_impl.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/pddl_world_state_property_impl_inline.h"
                # predicate
                "${CMAKE_CURRENT_SOURCE_DIR}/predicate.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/predicate.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/predicate_inline.h"
                # lexemes
                # base
                "${CMAKE_CURRENT_SOURCE_DIR}/base_lexeme.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/base_lexeme.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/base_lexeme_inline.h"
                # ptr
                "${CMAKE_CURRENT_SOURCE_DIR}/base_lexeme_ptr.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/base_lexeme_ptr.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/base_lexeme_ptr_inline.h"
                # on_demand_planner
                "${CMAKE_CURRENT_SOURCE_DIR}/../search/on_demand_planner.h"
                # subscriptions
                "${CMAKE_CURRENT_SOURCE_DIR}/npc_subscriptions_manager.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/npc_subscriptions_manager_inline.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/subscriptions_manager.h"
                "${CMAKE_CURRENT_SOURCE_DIR}/subscriptions_manager_inline.h"
                # ./
                "${CMAKE_CURRENT_SOURCE_DIR}/ai_entry_point.cpp"
)

target_link_libraries(ai PUBLIC
	core
)

set_target_properties(ai PROPERTIES FOLDER "engine/ai")
