project(animation_editor LANGUAGES CXX CSharp)

add_library(animation_editor SHARED
								# core
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_editor_library_linkage.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_editor_memory.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/editor_creator.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/editor_creator.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
								# include
								"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
								# channels
								"${CMAKE_CURRENT_SOURCE_DIR}/channels_editor.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/channels_editor.h"
								# files_view_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/channels_files_source.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/channels_files_source.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/channels_files_view_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/channels_files_view_panel.h"
								# document
								"${CMAKE_CURRENT_SOURCE_DIR}/channels_document.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/channels_document.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/channels_document.resx"
								# layout_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/channels_layout_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/channels_layout_panel.h"
								# resources
								"${CMAKE_CURRENT_SOURCE_DIR}/resources.resx"
								# images
								"${CMAKE_CURRENT_SOURCE_DIR}/images/images16x16.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/images/images16x16.png"
								"${CMAKE_CURRENT_SOURCE_DIR}/images/images24x24.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/images/images24x24.png"
								# viewer
								"${CMAKE_CURRENT_SOURCE_DIR}/viewer_editor.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/viewer_editor.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/viewer_editor_components.cpp"
								# document
								"${CMAKE_CURRENT_SOURCE_DIR}/viewer_document.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/viewer_document.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/viewer_document_animation.cpp"
								# hypergraph
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_hypergraph.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_hypergraph.h"
								# nodes_style_manager
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_node_style.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_node_style.h"
								# links_manager
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_connection_editor.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_connection_editor.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_link.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_link.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_link_timed.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_link_timed.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_link_weighted.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_link_weighted.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_links_manager.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_links_manager.h"
								# nodes
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_node_animation.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_node_animation.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_node_base.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_node_base.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_node_operator.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_node_operator.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_node_root.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_graph_node_root.h"
								# animation_node_playing_instance
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_playing_instance.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_playing_instance.h"
								# time_scale_instance
								"${CMAKE_CURRENT_SOURCE_DIR}/time_scale_instance.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/time_scale_instance.h"
								# animation_node_interpolator
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_interpolator.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_interpolator.h"
								# time_instance
								"${CMAKE_CURRENT_SOURCE_DIR}/time_instance.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/time_instance.h"
								# commands
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_add_delete_group.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_add_delete_group.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_add_delete_links.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_add_delete_links.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_change_lexeme.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_change_lexeme.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_change_property.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_change_property.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_move_nodes.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_move_nodes.h"
								# enum_defines
								"${CMAKE_CURRENT_SOURCE_DIR}/enum_defines.h"
								# models_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/models_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/models_panel.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/models_panel.resx"
								# toolbar
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_toolbar_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_toolbar_panel.h"
								# properties_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_properties_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_properties_panel.h"
								# time_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_time_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_time_panel.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_time_panel.resx"
								# detailed_options_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_detailed_options_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_detailed_options_panel.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_viewer_detailed_options_panel.resx"
								# lexeme_editor
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_lexeme_editor.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_lexeme_editor.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_lexeme_editor.resx"
								# animations_view_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/animations_view_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animations_view_panel.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animations_view_source.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animations_view_source.h"
								# animation_editor_contexts_manager
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_editor_contexts_manager.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_editor_contexts_manager.h"
								# animation_setup_manager
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_manager.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_manager.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_manager.resx"
								# animations_list_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/animations_list_files_source.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animations_list_files_source.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animations_list_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animations_list_panel.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animations_list_panel.resx"
								# animation_setup_document
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_document.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_document.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_document_components.cpp"
								# commands
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_channel_added_removed_commands.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_channel_added_removed_commands.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_item_added_removed_commands.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_item_added_removed_commands.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_property_changed_commands.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_property_changed_commands.h"
								# properties_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_properties_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_setup_properties_panel.h"
								# animated_model
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_managed_callback.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_model.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_model.h"
								# animation_editor
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_editor.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_editor.h"
								# form
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_editor_form.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_editor_form.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_editor_form.resx"
								# view_controls
								"${CMAKE_CURRENT_SOURCE_DIR}/actions.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/actions.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_helper.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_helper.h"
								# groups
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_editor.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_editor.h"
								# tollbar
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_toolbar.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_toolbar.h"
								# controllers_tree
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_controllers_tree.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_controllers_tree.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_controllers_tree_files_source.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_controllers_tree_files_source.h"
								# animations_tree
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_animations_tree.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_animations_tree.h"
								# document
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_document.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_document.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_document.resx"
								# commands
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_commands.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_commands.h"
								# intervals_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_intervals_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_intervals_panel.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_groups_intervals_panel.resx"
								# animation_node_interval_instance
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_interval_instance.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_interval_instance.h"
								# group_object
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_group_object.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_group_object.h"
								# learning_properties
								"${CMAKE_CURRENT_SOURCE_DIR}/learning_properties_form.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/learning_properties_form.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/learning_properties_form.resx"
								# node_clip
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_clip.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_clip.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_clip_instance.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_clip_instance.h"
								# interval
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_interval.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_node_interval.h"
								# animation_collections
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collections_editor.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collections_editor.h"
								# document
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collection_document.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collection_document.h"
								# commands
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collection_document_commands.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collection_document_commands.h"
								# animations_view_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_files_source.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_files_source.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_files_view_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_files_view_panel.h"
								# collections_view_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collection_files_source.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collection_files_source.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collections_view_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collections_view_panel.h"
								# document_base
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collections_document_base.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collections_document_base.h"
								# item
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collection_item.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_collection_item.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_item.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_item.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_item_type.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_single_item.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_single_item.h"
								# ./
								"${CMAKE_CURRENT_SOURCE_DIR}/animation_editor_entry_point.cpp"
)

target_link_libraries(animation_editor PRIVATE
	engine_pc_dx11
)

set_target_properties(animation_editor PROPERTIES FOLDER "engine/editors")
