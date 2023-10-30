project(editor_controls LANGUAGES CXX CSharp)

add_library(editor_controls SHARED
								# core
								"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
								# include
								"${CMAKE_CURRENT_SOURCE_DIR}/base_registry_key.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/../property_container_interface.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/../property_incrementable.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/../property_mouse_events.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/../property_value.h"
								# controls
								# hypergraph
								"${CMAKE_CURRENT_SOURCE_DIR}/drawing2d_utils.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/drawing2d_utils.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/hypergraph.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/hypergraph.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/hypergraph.resx"
								"${CMAKE_CURRENT_SOURCE_DIR}/hypergraph_defines.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/hypergraph_draw.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/hypergraph_node.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/hypergraph_node.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/hypergraph_node.resx"
								"${CMAKE_CURRENT_SOURCE_DIR}/hypergraph_node_draw.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/hypergraph_node_style.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/hypergraph_node_style.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/node_connection_point.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/node_connection_point.h"
								# tree_view
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_node.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_node.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_node_alphanum_comparer.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_node_alphanum_comparer.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_node_type.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_event_args.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_filter_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_filter_panel.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_filter_panel.resx"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_initialize.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_multiselection.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_search_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_search_panel.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_search_panel.resx"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_select_action.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_selection_event_args.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_source.h"
								# scalable_scroll_bar
								"${CMAKE_CURRENT_SOURCE_DIR}/scalable_scroll_bar.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/scalable_scroll_bar.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/scalable_scroll_bar.resx"
								"${CMAKE_CURRENT_SOURCE_DIR}/scroller_event_args.h"
								# drag_cursor_collection
								"${CMAKE_CURRENT_SOURCE_DIR}/drag_cursor_collection.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/drag_cursor_collection.h"
								# document_editor_base
								"${CMAKE_CURRENT_SOURCE_DIR}/delegates.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/document_editor_base.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/document_editor_base.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/document_event_args.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/i_document_editor_shortcut_receiver.h"
								# document_base
								"${CMAKE_CURRENT_SOURCE_DIR}/document_base.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/document_base.h"
								# file_view_panel_base
								"${CMAKE_CURRENT_SOURCE_DIR}/file_view_panel_base.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/file_view_panel_base.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/file_view_panel_base.resx"
								# item_properties_panel_base
								"${CMAKE_CURRENT_SOURCE_DIR}/item_properties_panel_base.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/item_properties_panel_base.h"
								# toolbar_panel_base
								"${CMAKE_CURRENT_SOURCE_DIR}/toolbar_panel_base.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/toolbar_panel_base.h"
								# dock_panel_serializer
								"${CMAKE_CURRENT_SOURCE_DIR}/dock_panel_serializer.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/dock_panel_serializer.h"
								# comparators
								"${CMAKE_CURRENT_SOURCE_DIR}/alphanum_comparator.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/alphanum_comparator.h"
								# mode_buttons
								"${CMAKE_CURRENT_SOURCE_DIR}/mode_button.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/mode_button.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/mode_strip_button.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/mode_strip_button.h"
								# ./
)

add_dependencies(editor_controls
	wpf_controls
)

set_target_properties(editor_controls PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v4.8")

set_target_properties(editor_controls PROPERTIES VS_DOTNET_REFERENCES
    "PresentationCore;PresentationFramework;System;System.Data;System.Drawing;System.Windows.Forms;System.Xml;WindowsBase;WindowsFormsIntegration;WeifenLuo.WinFormsUI.Docking;wpf_controls"
)

set_target_properties(editor_controls PROPERTIES FOLDER "engine/editors")
