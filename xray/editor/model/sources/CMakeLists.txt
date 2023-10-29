project(model_editor LANGUAGES CXX CSharp)

add_library(model_editor SHARED
							# core
							"${CMAKE_CURRENT_SOURCE_DIR}/editor_creator.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/editor_creator.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/model_editor_library_linkage.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/model_editor_memory.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
							# resources
							"${CMAKE_CURRENT_SOURCE_DIR}/resources.resx"
							# input_context
							"${CMAKE_CURRENT_SOURCE_DIR}/contexts_manager.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/contexts_manager.h"
							# windows
							# ide
							"${CMAKE_CURRENT_SOURCE_DIR}/ide.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/ide.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/ide.resx"
							# LOD view
							"${CMAKE_CURRENT_SOURCE_DIR}/lod_view_panel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/lod_view_panel.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/lod_view_panel.resx"
							# surfaces_list
							"${CMAKE_CURRENT_SOURCE_DIR}/property_grid_panel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/property_grid_panel.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/property_grid_panel.resx"
							# portals_list
							"${CMAKE_CURRENT_SOURCE_DIR}/object_portals_panel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/object_portals_panel.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/object_portals_panel.resx"
							# portal_generation_view
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_tree_control.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_tree_control.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_tree_control.resx"
							"${CMAKE_CURRENT_SOURCE_DIR}/portal_generation_view.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/portal_generation_view.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/portal_generation_view.resx"
							# sectors_view
							"${CMAKE_CURRENT_SOURCE_DIR}/sectors_controls.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sectors_controls.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sectors_controls.resx"
							"${CMAKE_CURRENT_SOURCE_DIR}/sectors_view.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sectors_view.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sectors_view.resx"
							"${CMAKE_CURRENT_SOURCE_DIR}/unmanaged_delegate.h"
							# objects
							# base
							"${CMAKE_CURRENT_SOURCE_DIR}/model_editor_object.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/model_editor_object.h"
							# solid_mesh
							"${CMAKE_CURRENT_SOURCE_DIR}/collision_primitive_item.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/collision_primitive_item.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_lods.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_mesh.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_mesh.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_mesh_collision.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_preview.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_properties.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_skeletal_mesh.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_solid_mesh.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_surface.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_surface.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/render_model_wrapper.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/render_model_wrapper.h"
							# speed_tree
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_speedtree.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_speedtree.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_speedtree_properties.cpp"
							# composite_visual
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_composite_visual.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_composite_visual.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_composite_visual_actions.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_composite_visual_item.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_composite_visual_properties.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_object_composite_visual_serialize.cpp"
							# portal_generation
							"${CMAKE_CURRENT_SOURCE_DIR}/generated_portal.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/generated_portal.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sector_generator_portal.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sector_generator_portal.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sectors_generator.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sectors_generator.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/spatial_sector.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/spatial_sector.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/triangles_mesh_utilities.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/triangles_mesh_utilities.h"
							# bsp_tree
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_portal_blank.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_portal_blank.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_tree.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_tree.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_tree_node.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_tree_node.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_tree_triangle.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_tree_triangle.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_tree_utilities.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_tree_utilities.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/bsp_typedefs.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/edge_utilities.h"
							# portal_edit
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_portal.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/edit_portal.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/float3_similar.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/portal_collision_object.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/portal_collision_object.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/portal_vertex_collision.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/portal_vertex_collision.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/portal_vertex_shift.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/portal_vertex_shift.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/triple.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/unique_name_helper.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/unique_name_helper.h"
							# editor
							"${CMAKE_CURRENT_SOURCE_DIR}/input_actions.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/input_actions.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/model_editor.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/model_editor.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/model_editor_actions.cpp"
							# ./
							"${CMAKE_CURRENT_SOURCE_DIR}/model_editor_entry_point.cpp"
)

target_link_libraries(model_editor PUBLIC
	engine_pc_dx11
)

set_property(TARGET model_editor PROPERTY VS_GLOBAL_RootNamespace xray.model_editor)

set_target_properties(model_editor PROPERTIES FOLDER "engine/editors")

add_dependencies(model_editor editor_base editor_controls)
