project(particle_editor LANGUAGES CXX CSharp)

add_library(particle_editor SHARED
								# core
								"${CMAKE_CURRENT_SOURCE_DIR}/editor_creator.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/editor_creator.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_editor_library_linkage.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_editor_memory.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
								# include
								"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
								# resources
								"${CMAKE_CURRENT_SOURCE_DIR}/resources.resx"
								# images
								"${CMAKE_CURRENT_SOURCE_DIR}/images/images104x8.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/images/images104x8.png"
								"${CMAKE_CURRENT_SOURCE_DIR}/images/images30x25.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/images/images30x25.png"
								"${CMAKE_CURRENT_SOURCE_DIR}/images/images92x25.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/images/images92x25.png"
								"${CMAKE_CURRENT_SOURCE_DIR}/images/images9x9.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/images/images9x9.png"
								"${CMAKE_CURRENT_SOURCE_DIR}/images/particle_editor_images16x16.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/images/particle_editor_images16x16.png"
								# particle_editor
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_editor.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_editor.h"
								# particle_graph
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_document.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_document.h"
								# particle_hypergraph
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node_collection.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node_collection.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node_style.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node_style.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_hypergraph.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_hypergraph.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_node_connection_editor.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_node_connection_editor.h"
								# particle_graph_node
								# particle_node_types
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node_action.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node_action.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node_emitter.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node_emitter.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node_event.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node_event.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node_property.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_graph_node_property.h"
								# particle_time_based_layout_props
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_time_based_layout_props.h"
								# particle_links_manager
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_links_manager.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_links_manager.h"
								# particle_node_properties
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_node_properties.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_node_properties.h"
								# particle_property_holders
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_source_property_holder.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_source_property_holder.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_time_parameters_holder.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_time_parameters_holder.h"
								# particle_property_type_holders
								# particle_graph_commands
								"${CMAKE_CURRENT_SOURCE_DIR}/add_delete_group_nodes.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/add_delete_group_nodes.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/change_node_index.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/change_node_index.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/disable_group_nodes.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/disable_group_nodes.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/move_node.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/move_node.h"
								# particle_groups_source
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_groups_source.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_groups_source.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_system_config_wrapper.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_tree_node_holder.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_tree_node_holder.h"
								# particle_editor_panels
								# particle_source_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_source_nodes_container.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_source_nodes_container.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_sources_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_sources_panel.h"
								# particle_time_based_layout_panel
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_time_based_layout_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_time_based_layout_panel.h"
								# particle_data_sources
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_sources_panel.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_sources_panel.h"
								# particle_data_sources_holder
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_sources_holder.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_sources_holder.h"
								# particle_data_source_types
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_source_base.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_source_color.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_source_color.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_source_color_matrix.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_source_color_matrix.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_source_float.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_source_float.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_source_float3.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_data_source_float3.h"
								# particle_contexts_manager
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_contexts_manager.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_contexts_manager.h"
								# view controls
								"${CMAKE_CURRENT_SOURCE_DIR}/property_visualizers.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/property_visualizers.h"
								"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_helper.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_helper.h"
								# actions
								"${CMAKE_CURRENT_SOURCE_DIR}/input_actions.cpp"
								"${CMAKE_CURRENT_SOURCE_DIR}/input_actions.h"
								# ./
								"${CMAKE_CURRENT_SOURCE_DIR}/particle_editor_entry_point.cpp"
)

target_link_libraries(particle_editor PUBLIC
	engine_pc_dx11
)

set_property(TARGET particle_editor PROPERTY VS_GLOBAL_RootNamespace particle_editor)

set_target_properties(particle_editor PROPERTIES FOLDER "engine/editors")

add_dependencies(particle_editor editor_base editor_controls)
