project(dialog_editor LANGUAGES CXX CSharp)

add_library(dialog_editor SHARED
							# document
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_document.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_document.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_document.resx"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_document_node_factory.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_hypergraph.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_hypergraph.h"
							# commands
							"${CMAKE_CURRENT_SOURCE_DIR}/command_add_delete_group.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/command_add_delete_group.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/command_add_delete_links.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/command_add_delete_links.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/command_change_node_properties.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/command_change_node_properties.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/command_change_node_string_table.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/command_change_node_string_table.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/command_move_nodes.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/command_move_nodes.h"
							# links_manager
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_graph_link.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_graph_link.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_links_manager.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_links_manager.h"
							# node
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_graph_node_base.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_graph_node_base.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_graph_node_dialog.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_graph_node_dialog.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_graph_node_layout.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_graph_node_layout.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_graph_node_phrase.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_graph_node_phrase.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_graph_node_style.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_graph_node_style.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_node_connection_editor.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_node_connection_editor.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_node_hypergraph.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_node_hypergraph.h"
							# dialog_manager
							"${CMAKE_CURRENT_SOURCE_DIR}/dialogs_manager.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialogs_manager.h"
							# objects
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_node_base.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_node_base.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_phrase.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_phrase.h"
							# editor
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_container.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_container.h"
							# game
							"${CMAKE_CURRENT_SOURCE_DIR}/game_dialog.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/game_dialog.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/game_dialog_container.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/game_dialog_container.h"
							# link
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_link.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_link.h"
							# expression
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_expression.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_expression.h"
							# operations
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_operation.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_operation.h"
							# operations
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_operation_and.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_operation_and.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_operation_not.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_operation_not.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_operation_or.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_operation_or.h"
							# precondition
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_precondition.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_precondition.h"
							# action
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_action.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_action.h"
							# cookers
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_cooker.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_cooker.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor_resource.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor_resource.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor_resources_cooker.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor_resources_cooker.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/game_dialog_cooker.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/game_dialog_cooker.h"
							# string_tables
							"${CMAKE_CURRENT_SOURCE_DIR}/string_table_id.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/string_table_id.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/string_table_id_options.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/string_table_id_options.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/string_table_ids_storage.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/string_table_ids_storage.h"
							# core
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor_components.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor_form.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor_form.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor_form.resx"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor_library_linkage.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor_memory.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/editor_creator.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/editor_creator.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
							# include
							"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
							# panels
							# toolbar
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_control_panel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_control_panel.h"
							# files_viewer
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_files_source.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_files_source.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_files_view_panel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_files_view_panel.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_files_view_panel.resx"
							# properties
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_item_properties_panel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_item_properties_panel.h"
							# languages
							"${CMAKE_CURRENT_SOURCE_DIR}/add_language_form.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/add_language_form.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/add_language_form.resx"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_language_panel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_language_panel.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_language_panel.resx"
							"${CMAKE_CURRENT_SOURCE_DIR}/remove_language_form.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/remove_language_form.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/remove_language_form.resx"
							# precondition_action_editor
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_link_action.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_link_action.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_link_precondition.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_link_precondition.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_precondition_action_editor.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_precondition_action_editor.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_precondition_action_editor.resx"
							# text_editor
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_text_editor.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_text_editor.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_text_editor.resx"
							# export_import
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_export_form.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_export_form.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_export_form.resx"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_import_form.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_import_form.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_import_form.resx"
							"${CMAKE_CURRENT_SOURCE_DIR}/excel_file.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/excel_file.h"
							# resources
							#"${CMAKE_CURRENT_SOURCE_DIR}/resources.resx"
							# images
							"${CMAKE_CURRENT_SOURCE_DIR}/images/images70x53.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/images/images70x53.png"
							# dialog_editor_contexts_manager
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor_contexts_manager.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor_contexts_manager.h"
							# ./
							"${CMAKE_CURRENT_SOURCE_DIR}/dialog_editor_entry_point.cpp"
)

target_link_libraries(dialog_editor PUBLIC
	engine_pc_dx11
	editor_base
)

set_target_properties(dialog_editor PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v4.0")

set_target_properties(dialog_editor PROPERTIES VS_DOTNET_REFERENCES
    "Microsoft.Office.Interop.Excel;PresentationCore;PresentationFramework;System;System.Data;System.Drawing;System.Windows.Forms;System.Xml;System.Xaml;WindowsBase;WindowsFormsIntegration;xray_editor_base.dll;xray_editor_controls.dll"
)

set_target_properties(dialog_editor PROPERTIES FOLDER "engine/editors")
