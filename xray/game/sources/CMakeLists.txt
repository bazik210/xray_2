project(game LANGUAGES CXX)

add_library(game STATIC
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_library_linkage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_memory.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					# game
					# objects
					# solid_visual
					"${CMAKE_CURRENT_SOURCE_DIR}/object_solid_visual.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_solid_visual.h"
					# light
					"${CMAKE_CURRENT_SOURCE_DIR}/light_behaviour.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/light_behaviour.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_light.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_light.h"
					# object
					"${CMAKE_CURRENT_SOURCE_DIR}/object.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_cooker.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_cooker.h"
					# track
					"${CMAKE_CURRENT_SOURCE_DIR}/object_track.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_track.h"
					# sound
					"${CMAKE_CURRENT_SOURCE_DIR}/object_sound.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_sound.h"
					# camera
					"${CMAKE_CURRENT_SOURCE_DIR}/camera_behaviour.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/camera_behaviour.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/camera_director.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/camera_director.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_camera.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_camera.h"
					# free fly
					"${CMAKE_CURRENT_SOURCE_DIR}/free_fly_camera.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/free_fly_camera.h"
					# event_generator
					"${CMAKE_CURRENT_SOURCE_DIR}/event_generator_behaviour.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/event_generator_behaviour.h"
					# environment
					"${CMAKE_CURRENT_SOURCE_DIR}/object_environment.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_environment.h"
					# decal
					"${CMAKE_CURRENT_SOURCE_DIR}/object_decal.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_decal.h"
					# actor
					"${CMAKE_CURRENT_SOURCE_DIR}/actor.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/actor.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/actor_controller.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/actor_input_controller.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/actor_input_controller.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/test_anim_object.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/test_anim_object.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/actor_hud.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/actor_hud.h"
					# old actor
					"${CMAKE_CURRENT_SOURCE_DIR}/hud.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/hud.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/hud_input.cpp"
					# collision_geometry
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision_geometry.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision_geometry.h"
					# proximity_trigger
					"${CMAKE_CURRENT_SOURCE_DIR}/object_proximity_trigger.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_proximity_trigger.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/proximity_trigger_behaviour.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/proximity_trigger_behaviour.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/proximity_trigger_input_handler.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/proximity_trigger_input_handler.h"
					# sky
					"${CMAKE_CURRENT_SOURCE_DIR}/object_sky.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_sky.h"
					# values_storage
					"${CMAKE_CURRENT_SOURCE_DIR}/object_values_storage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_values_storage.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/values_storage_behaviour.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/values_storage_behaviour.h"
					# composite_object
					"${CMAKE_CURRENT_SOURCE_DIR}/composite_object_behaviour.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/composite_object_behaviour.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_composite.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_composite.h"
					# volumetric_sound
					"${CMAKE_CURRENT_SOURCE_DIR}/object_volumetric_sound.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_volumetric_sound.h"
					# weapon
					"${CMAKE_CURRENT_SOURCE_DIR}/weapon.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/weapon.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/weapon_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/weapon_cook.h"
					# wire
					"${CMAKE_CURRENT_SOURCE_DIR}/object_wire.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_wire.h"
					# cells
					# cell
					"${CMAKE_CURRENT_SOURCE_DIR}/cell.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/cell.h"
					# manager
					"${CMAKE_CURRENT_SOURCE_DIR}/cell_manager.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/cell_manager.h"
					# cooker
					"${CMAKE_CURRENT_SOURCE_DIR}/cell_cooker.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/cell_cooker.h"
					# stats
					"${CMAKE_CURRENT_SOURCE_DIR}/stats.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/stats.h"
					# npc_stats
					"${CMAKE_CURRENT_SOURCE_DIR}/npc_stats.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/npc_stats.h"
					# stats_graph
					"${CMAKE_CURRENT_SOURCE_DIR}/stats_graph.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/stats_graph.h"
					# ai_game_objects
					# building
					"${CMAKE_CURRENT_SOURCE_DIR}/building_object.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/building_object.h"
					# npc
					# human
					"${CMAKE_CURRENT_SOURCE_DIR}/human_npc.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/human_npc.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/human_npc_animation_controller_planner.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/human_npc_animation_controller_planner.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/human_npc_animation_controller.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/human_npc_animations.cpp"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/human_npc_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/human_npc_cook.h"
					# game_material_visibility_parameters
					"${CMAKE_CURRENT_SOURCE_DIR}/game_material_visibility_parameters.h"
					# npc_behaviours
					"${CMAKE_CURRENT_SOURCE_DIR}/npc_empty_switcher_behaviour.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/npc_empty_switcher_behaviour.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/npc_play_animation_behaviour.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/npc_play_animation_behaviour.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/npc_reach_position_behaviour.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/npc_reach_position_behaviour.h"
					# composite_building
					"${CMAKE_CURRENT_SOURCE_DIR}/composite_building.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/composite_building.h"
					# collision_object
					"${CMAKE_CURRENT_SOURCE_DIR}/ai_collision_object.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/ai_collision_object.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/collision_object_types.h"
					# weapon
					"${CMAKE_CURRENT_SOURCE_DIR}/object_weapon.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_weapon.h"
					# project
					"${CMAKE_CURRENT_SOURCE_DIR}/game_project.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_project.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_cooker.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_cooker.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_cooker_simple.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_cooker_simple.h"
					# sound_player
					"${CMAKE_CURRENT_SOURCE_DIR}/ai_sound_player.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/ai_sound_player.cpp"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_player_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_player_cook.h"
					# sound_position_calculator
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_position_calculator.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_position_calculator.h"
					# animated_model_instance
					"${CMAKE_CURRENT_SOURCE_DIR}/animated_model_instance.h"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/animated_model_instance_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/animated_model_instance_cook.h"
					# jobs,events etc
					"${CMAKE_CURRENT_SOURCE_DIR}/event_manager.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/event_manager.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_action.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_fwd_defines.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_behaviour.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_behaviour.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_scene.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_scene.h"
					# scenes
					"${CMAKE_CURRENT_SOURCE_DIR}/game_scene.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_scene.h"
					# game_world
					"${CMAKE_CURRENT_SOURCE_DIR}/game_world.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_world.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_world_input.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/memory_allocation_test.cpp"
					# main_menu
					"${CMAKE_CURRENT_SOURCE_DIR}/main_menu.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/main_menu.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/main_menu_input.cpp"
					# lobby_menu
					"${CMAKE_CURRENT_SOURCE_DIR}/lobby_menu.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/lobby_menu.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/lobby_menu_input.cpp"
					# bullet_manager
					"${CMAKE_CURRENT_SOURCE_DIR}/bullet.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/bullet.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/bullet_manager.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/bullet_manager.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/bullet_manager_debug_render.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/bullet_manager_input_handler.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/bullet_manager_input_handler.h"
					# animations_selector
					"${CMAKE_CURRENT_SOURCE_DIR}/animations_selector.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/animations_selector.h"
					# animation_controllers
					# base
					"${CMAKE_CURRENT_SOURCE_DIR}/base_animation_controller.h"
					# single_position
					"${CMAKE_CURRENT_SOURCE_DIR}/single_position_animation_controller.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/single_position_animation_controller.h"
					# parameters
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_controller_parameters.h"
					# simple_animation
					"${CMAKE_CURRENT_SOURCE_DIR}/simple_animation_controller.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/simple_animation_controller.h"
					# flash_factory
					"${CMAKE_CURRENT_SOURCE_DIR}/flash_factory.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/flash_factory.h"
					# key_binder
					"${CMAKE_CURRENT_SOURCE_DIR}/key_binder.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/key_binder.h"
					# help functions
					"${CMAKE_CURRENT_SOURCE_DIR}/help_functions.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/help_functions.h"
					# persistent
					"${CMAKE_CURRENT_SOURCE_DIR}/game.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/game.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_initialize.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_render.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_unused.cpp"
					# input_handlers
					# game_exit
					"${CMAKE_CURRENT_SOURCE_DIR}/game_exit_handler.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_exit_handler.h"
					# npc_manipulation
					"${CMAKE_CURRENT_SOURCE_DIR}/npc_manipulation_handler.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/npc_manipulation_handler.h"
					# network
					"${CMAKE_CURRENT_SOURCE_DIR}/game_net_client.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/game_net_client.h"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/game_entry_point.cpp"
)

target_include_directories(game PRIVATE
#	${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Include
#	${THIRDPARTY_DIR_INCLUDE_PATH}/scaleform/Src
	"${DXSDK_DIR_PATH}/Include"
)

target_link_libraries(game PUBLIC
	debugging
)

set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/flash_factory.cpp PROPERTY VS_SETTINGS "ExcludedFromBuild=true")
set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/flash_factory.h PROPERTY VS_SETTINGS "ExcludedFromBuild=true")
set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/hud.cpp PROPERTY VS_SETTINGS "ExcludedFromBuild=true")
set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/hud.h PROPERTY VS_SETTINGS "ExcludedFromBuild=true")
set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/hud_input.cpp PROPERTY VS_SETTINGS "ExcludedFromBuild=true")

set_target_properties(game PROPERTIES FOLDER "stalker2")
