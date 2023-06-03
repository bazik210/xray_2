project(editor LANGUAGES CXX CSharp)

add_library(editor SHARED
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_library_linkage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_memory.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/lua_config_value_editor.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					# include
					"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
					# world
					"${CMAKE_CURRENT_SOURCE_DIR}/console_wrapper.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/console_wrapper.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_world.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_world.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_world_inline.h"
					# collision
					"${CMAKE_CURRENT_SOURCE_DIR}/collision_object_dynamic.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/collision_object_dynamic.h"
					# project
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_cell_manager.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_cell_manager.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_in_project_chooser.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_in_project_chooser.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/project.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/project.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_defines.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_export.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_items.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_items.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_items_serialize.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_selection.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_serialize.cpp"
					# ui
					"${CMAKE_CURRENT_SOURCE_DIR}/object_in_project_chooser_form.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_in_project_chooser_form.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_in_project_chooser_form.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_tab.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_tab.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_tab_drag_drop.cpp"
					# tools&objects
					# base
					"${CMAKE_CURRENT_SOURCE_DIR}/object_base.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_base.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_base_transform.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/objects_library.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/objects_library.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/objects_library_serialize.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_base.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_base.h"
					# ui
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/tools_tab.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tools_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/tools_tab.resx"
					# solid_visual
					"${CMAKE_CURRENT_SOURCE_DIR}/object_skeleton_visual.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_skeleton_visual.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_solid_visual.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_solid_visual.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_solid_visual.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_solid_visual.h"
					# ui
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_loading_dialog.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_loading_dialog.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_loading_dialog.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_tool_tab.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_tool_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_tool_tab.resx"
					# shared
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_window_storage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_window_storage.h"
					# light
					"${CMAKE_CURRENT_SOURCE_DIR}/object_light.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_light.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_light_draw.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_light_props.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_light.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_light.h"
					# ui
					"${CMAKE_CURRENT_SOURCE_DIR}/light_tool_tab.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/light_tool_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/light_tool_tab.resx"
					# composite
					"${CMAKE_CURRENT_SOURCE_DIR}/object_composite.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_composite.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_composite_edit.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_composite_serialize.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_in_compound_chooser.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_in_compound_chooser.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_composite.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_composite.h"
					# ui
					"${CMAKE_CURRENT_SOURCE_DIR}/compound_tool_tab.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/compound_tool_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/compound_tool_tab.resx"
					# object_in_compound_chooser_form
					"${CMAKE_CURRENT_SOURCE_DIR}/object_in_compound_chooser_form.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_in_compound_chooser_form.h"
					# object properties
					"${CMAKE_CURRENT_SOURCE_DIR}/attribute.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/attribute.h"
					# terrain
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_object.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_object.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_object_collision.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_object_core.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_object_drawing.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_object_export.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_object_helpers.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_object_import.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_object_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_object_serialize.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_quad.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_quad.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_terrain.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_terrain.h"
					# commands
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_command.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_command.h"
					# terrain_collision
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_collision_geometry.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_collision_geometry.h"
					# ui
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_copy_nodes_form.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_copy_nodes_form.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_copy_nodes_form.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_core_form.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_core_form.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_core_form.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_import_form.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_import_form.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_import_form.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_painter_control_tab.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_painter_control_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_painter_control_tab.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_tool_tab.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_tool_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_tool_tab.resx"
					# modifiers
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_control_cursor.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_control_cursor.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_bump.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_bump.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_control.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_control.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_flatten.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_flatten.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_painter.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_painter.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_raise_lower.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_raise_lower.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_smooth.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_smooth.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_uv_relax.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_modifier_uv_relax.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_selection_control.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/terrain_selection_control.h"
					# particle
					"${CMAKE_CURRENT_SOURCE_DIR}/object_particle.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_particle.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_particle.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_particle.h"
					# object_painter
					"${CMAKE_CURRENT_SOURCE_DIR}/object_painter_control.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_painter_control.h"
					# ui
					"${CMAKE_CURRENT_SOURCE_DIR}/object_painter_tool_tab.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_painter_tool_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_painter_tool_tab.resx"
					# object_inspector
					"${CMAKE_CURRENT_SOURCE_DIR}/object_inspector_tab.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_inspector_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_inspector_tab.resx"
					# misc
					"${CMAKE_CURRENT_SOURCE_DIR}/object_camera.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_camera.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_human_npc.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_human_npc.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_animations.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_animations.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_misc.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_misc.h"
					# ui
					"${CMAKE_CURRENT_SOURCE_DIR}/misc_tool_tab.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/misc_tool_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/misc_tool_tab.resx"
					# object_patrol_graph
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph.h"
					# actions
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_action.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_add_look_point_action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_add_look_point_action.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_change_edge_direction_action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_change_edge_direction_action.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_complete_graph_action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_complete_graph_action.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_delete_part_action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_delete_part_action.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_link_nodes_action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_link_nodes_action.h"
					# commands
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_add_edge_command.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_add_edge_command.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_add_look_point_command.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_add_look_point_command.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_add_node_command.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_add_node_command.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_change_edge_direction_command.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_change_edge_direction_command.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_remove_edge_command.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_remove_edge_command.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_remove_look_point_command.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_remove_look_point_command.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_remove_node_command.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_remove_node_command.h"
					# parts
					# node
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_node.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_node.h"
					# edge
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_edge.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_edge.h"
					# look_point
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_look_point.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_look_point.h"
					# base
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_part.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_part.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_part_collision.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_part_collision.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_part_transform_data.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_patrol_graph_part_transform_data.h"
					# object_track
					"${CMAKE_CURRENT_SOURCE_DIR}/object_track.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_track.h"
					# commands
					"${CMAKE_CURRENT_SOURCE_DIR}/object_track_set_key_command.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_track_set_key_command.h"
					# object_collision_geometry
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision_geometry.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision_geometry.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision_geometry_mesh.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision_geometry_mesh.h"
					# commands
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision_geometry_add_remove_mesh_commands.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision_geometry_add_remove_mesh_commands.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision_geometry_mesh_move_command.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision_geometry_mesh_move_command.h"
					# actions
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision_geometry_set_plane_action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_collision_geometry_set_plane_action.h"
					# object_proximity_trigger
					"${CMAKE_CURRENT_SOURCE_DIR}/object_proximity_trigger.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_proximity_trigger.h"
					# object_values_storage
					"${CMAKE_CURRENT_SOURCE_DIR}/object_values_storage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_values_storage.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_values_storage_edit_form.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_values_storage_edit_form.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_values_storage_edit_form.resx"
					# wire_set
					"${CMAKE_CURRENT_SOURCE_DIR}/object_wire_set.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_wire_set.h"
					# environment
					"${CMAKE_CURRENT_SOURCE_DIR}/object_environment.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_environment.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_environment_props.cpp"
					# sky
					"${CMAKE_CURRENT_SOURCE_DIR}/object_sky.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_sky.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_sky_props.cpp"
					# decal
					"${CMAKE_CURRENT_SOURCE_DIR}/object_decal.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_decal.h"
					# environment_settings
					"${CMAKE_CURRENT_SOURCE_DIR}/object_environment_settings.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_environment_settings.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_environment_settings_key.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_environment_settings_key.h"
					# scripting
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_scripting.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_scripting.h"
					# ui
					"${CMAKE_CURRENT_SOURCE_DIR}/scripting_tool_tab.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/scripting_tool_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/scripting_tool_tab.resx"
					# scripting_objects
					"${CMAKE_CURRENT_SOURCE_DIR}/object_job.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_job.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_logic.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_logic.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_scene.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_scene.h"
					# logic_event
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_link_chooser_form.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_link_chooser_form.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_link_chooser_form.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_properties.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_properties.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_properties.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_subscriber.h"
					# job_behaviour
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_base.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_base.h"
					# job_behaviours
					# logic_event_property
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_properties_container.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_properties_container.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_property_holder.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_property_holder.h"
					# job_behaviour_wrapper
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_wrapper.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_wrapper.h"
					# logic_event_actions
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_action_base.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_action_raise_event.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_action_raise_event.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_action_switch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_action_switch.h"
					# behaviours_by_objects
					# timing_behaviours
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_event_generator.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_event_generator.h"
					# light_behaviours
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_light_anim.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_light_anim.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_light_static.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_light_static.h"
					# camera_director_behaviours
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_camera_director_default.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_camera_director_default.h"
					# camera_behaviours
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_camera_from_object.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_camera_from_object.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_camera_track.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_camera_track.h"
					# proximity_trigger_behaviours
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_proximity_trigger_default.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_proximity_trigger_default.h"
					# composite_behaviours
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_composite_default.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_composite_default.h"
					# values_storage_behaviours
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_values_storage_default.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_values_storage_default.h"
					# npc_behaviours
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_npc_patrol.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_npc_patrol.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_npc_play_animation.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/job_behaviour_npc_play_animation.h"
					# scenes_view
					# logic_scene_actions
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_action_start_scene.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_event_action_start_scene.h"
					# logic_scene_events_container
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_scene_event_property_holder.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_scene_event_property_holder.h"
					# commands
					"${CMAKE_CURRENT_SOURCE_DIR}/command_delete_in_logic_hypergraph.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_delete_in_logic_hypergraph.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_delete_in_scene_logic_hypergraph.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_delete_in_scene_logic_hypergraph.h"
					# behaviour_editor
					"${CMAKE_CURRENT_SOURCE_DIR}/behaviour_editor.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/behaviour_editor.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/behaviour_editor_form.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/behaviour_editor_form.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/behaviour_editor_form.resx"
					# generic_behaviour
					"${CMAKE_CURRENT_SOURCE_DIR}/generic_behaviour.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/generic_behaviour.h"
					# sound
					"${CMAKE_CURRENT_SOURCE_DIR}/object_sound.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_sound.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_sound.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_sound.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/volumetric_sound.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/volumetric_sound.h"
					# ui
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_debug_mode_strip_button.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_tool_tab.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_tool_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/sound_tool_tab.resx"
					# speedtree
					"${CMAKE_CURRENT_SOURCE_DIR}/object_speedtree.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_speedtree.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_speedtree.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/tool_speedtree.h"
					# commands
					"${CMAKE_CURRENT_SOURCE_DIR}/command_add_library_object.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_add_library_object.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_create_remove_link.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_create_remove_link.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_delete_object.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_delete_object.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_drop_objects.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_drop_objects.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_paste_objects.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_paste_objects.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_place_objects.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_place_objects.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_reparent_items.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_reparent_items.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_select.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_select.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_set_object_transform.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_set_object_transform.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_snap_base.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_snap_base.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_snap_objects.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_snap_objects.h"
					# actions
					"${CMAKE_CURRENT_SOURCE_DIR}/actions.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/actions.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/input_actions.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/input_actions.h"
					# editor_contexts_manager
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_contexts_manager.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_contexts_manager.h"
					# animated_model_instance
					"${CMAKE_CURRENT_SOURCE_DIR}/animated_model_instance.h"
					# cook
					"${CMAKE_CURRENT_SOURCE_DIR}/animated_model_instance_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/animated_model_instance_cook.h"
					# windows
					# about_dialog
					"${CMAKE_CURRENT_SOURCE_DIR}/about_dialog.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/about_dialog.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/about_dialog.resx"
					# ide
					"${CMAKE_CURRENT_SOURCE_DIR}/ide_log_form.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/ide_log_form.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/ide_log_form.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/window_ide.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/window_ide.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/window_ide.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/window_ide_hook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/window_ide_serialize.cpp"
					# library_object
					"${CMAKE_CURRENT_SOURCE_DIR}/library_objects_tree.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/library_objects_tree.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/library_objects_tree.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/select_library_class.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/select_library_class.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/select_library_class_form.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/select_library_class_form.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/select_library_class_form.resx"
					# options
					# options_pages
					"${CMAKE_CURRENT_SOURCE_DIR}/options_page_general.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/options_page_general.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/options_page_general.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/options_page_project.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/options_page_project.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/options_page_project.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/options_page_view_panel.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/options_page_view_panel.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/options_page_view_panel.resx"
					# connection_editor
					"${CMAKE_CURRENT_SOURCE_DIR}/property_connection_editor.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_connection_editor.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_connection_editor.resx"
					# scene_graph
					"${CMAKE_CURRENT_SOURCE_DIR}/scene_graph_node.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/scene_graph_node.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/scene_graph_node_style.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/scene_graph_node_style.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/scene_graph_tab.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/scene_graph_tab.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/scene_graph_tab.resx"
					# logic_editor
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_editor_panel.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_editor_panel.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/logic_editor_panel.resx"
					# animation_curve_editor
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_curve_editor_panel.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_curve_editor_panel.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/animation_curve_editor_panel.resx"
					# triangulation_tester
					"${CMAKE_CURRENT_SOURCE_DIR}/triangulation_tester.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/triangulation_tester.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/triangulation_tester_canvas.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/triangulation_tester_canvas.h"
					# level_editor
					"${CMAKE_CURRENT_SOURCE_DIR}/level_editor.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/level_editor.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/level_editor_actions.cpp"
					# project_browser_dialog
					"${CMAKE_CURRENT_SOURCE_DIR}/project_browser_dialog.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_browser_dialog.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/project_browser_dialog.resx"
					# editor_controls
					"${CMAKE_CURRENT_SOURCE_DIR}/command_apply_control_transform.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_apply_control_transform.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/le_transform_control_helper.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/le_transform_control_helper.h"
					# resources
					"${CMAKE_CURRENT_SOURCE_DIR}/resources.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/resources.rc"
					"${CMAKE_CURRENT_SOURCE_DIR}/resources.resx"
					# cursors
					"${CMAKE_CURRENT_SOURCE_DIR}/cursors/transform_general.cur"
					# images
					"${CMAKE_CURRENT_SOURCE_DIR}/images/folder_icons.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/images/folder_icons16x16.png"
					"${CMAKE_CURRENT_SOURCE_DIR}/images/folder_icons32x32.png"
					"${CMAKE_CURRENT_SOURCE_DIR}/images/folder_icons64x64.png"
					# resource_editors
					# texture_editor
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_document.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_document.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_document.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_document_options.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_document_preview.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_editor.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_editor.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_editor_binders.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_editor_resource.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_editor_resource.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_editor_source.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_editor_source.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_options.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_tga_to_argb_cook.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/texture_tga_to_argb_cook.h"
					# material_editor
					"${CMAKE_CURRENT_SOURCE_DIR}/material_editor.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_editor.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_editor_actions.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_editor_preview.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_editor_resource_specific.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_editor_source.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_editor_source.h"
					# effect_selector
					"${CMAKE_CURRENT_SOURCE_DIR}/effect_selector.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/effect_selector.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/effect_selector.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/effect_selector_item_comparator.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/effect_selector_item_comparator.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/effect_selector_source.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/effect_selector_source.h"
					# stage_selector
					"${CMAKE_CURRENT_SOURCE_DIR}/stage_selector.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/stage_selector.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/stage_selector_item_comparator.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/stage_selector_item_comparator.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/stage_selector_source.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/stage_selector_source.h"
					# material
					"${CMAKE_CURRENT_SOURCE_DIR}/material.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/material.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_callback_binders.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_effect.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_effect.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_property.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_property.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_property_base.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_property_read_writers.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_stage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_stage.h"
					# material_instance
					"${CMAKE_CURRENT_SOURCE_DIR}/material_instance.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/material_instance.h"
					# base
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_editor.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_editor.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_editor.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_editor_cancel.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_editor_cancel.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_editor_cancel.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_editor_commit.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_editor_commit.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_editor_commit.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_editor_resource.h"
					# render_options_editor
					"${CMAKE_CURRENT_SOURCE_DIR}/render_options_editor.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/render_options_editor.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/render_options_editor_resource.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/render_options_editor_resource.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/render_options_editor_source.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/render_options_editor_source.h"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_entry_point.cpp"
)

target_link_libraries(editor PUBLIC
	dialog_editor
	model_editor
	animation_editor
	particle_editor
	sound_editor
	editor_controls
)

set_target_properties(editor PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")

set_target_properties(editor PROPERTIES VS_DOTNET_REFERENCES
    "bugtrapN-x64;Microsoft.Office.Interop.Excel;Microsoft.VisualBasic;PresentationCore;PresentationFramework;System;System.Core;System.Data;System.Design;System.Drawing;System.Windows.Forms;System.Xml;WeifenLuo.WinForms.UI.Docking;WindowsBase;WindowsFormsIntegration;wpf_controls;WPFToolkit.Extended;xray_animation_editor;xray_dialog_editor;xray_editor_base;xray_editor_controls;xray_engine_pc_dx11;xray_model_editor;xray_particle_editor;xray_sound_editor"
)

set_target_properties(editor PROPERTIES FOLDER "engine/editors")
