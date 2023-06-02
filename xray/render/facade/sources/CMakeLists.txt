project(renderer_facade LANGUAGES CXX)

add_library(renderer_facade STATIC
							# core
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/render_facade_library_linkage.cpp"
							# include
							"${CMAKE_CURRENT_SOURCE_DIR}/../../api.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../../world.h"
							# facade
							"${CMAKE_CURRENT_SOURCE_DIR}/../base_command.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../common_types.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../cull_mode.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../decal_properties.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../decal_properties_inline.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../material_effects_instance_cook_data.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../model.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../particles.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../render_stage_types.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../scene_view_mode.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../terrain_base.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../vertex_input_type.h"
							# one_way_render_channel
							"${CMAKE_CURRENT_SOURCE_DIR}/../one_way_render_channel.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../one_way_render_channel_inline.h"
							# renderers
							"${CMAKE_CURRENT_SOURCE_DIR}/../debug_renderer.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../editor_renderer.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../engine_renderer.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../game_renderer.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../scene_renderer.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../ui_renderer.h"
							# world
							"${CMAKE_CURRENT_SOURCE_DIR}/render_facade_world.cpp"
							# one_way_render_channel
							"${CMAKE_CURRENT_SOURCE_DIR}/one_way_render_channel.cpp"
							# render_stage_types
							"${CMAKE_CURRENT_SOURCE_DIR}/render_stage_types.cpp"
							# renderers
							# game_renderer
							"${CMAKE_CURRENT_SOURCE_DIR}/game_renderer.cpp"
							# debug_renderer
							"${CMAKE_CURRENT_SOURCE_DIR}/debug_renderer.cpp"
							# commands
							# draw_triangles
							"${CMAKE_CURRENT_SOURCE_DIR}/debug_draw_triangles_command.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/debug_draw_triangles_command.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/debug_draw_triangles_command_inline.h"
							# draw_lines
							"${CMAKE_CURRENT_SOURCE_DIR}/debug_draw_lines_command.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/debug_draw_lines_command.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/debug_draw_lines_command_inline.h"
							# editor_renderer
							"${CMAKE_CURRENT_SOURCE_DIR}/editor_renderer.cpp"
							# commands
							# draw_screen_lines
							"${CMAKE_CURRENT_SOURCE_DIR}/editor_draw_screen_lines_command.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/editor_draw_screen_lines_command.h"
							# draw_3d_screen_lines
							"${CMAKE_CURRENT_SOURCE_DIR}/editor_draw_3d_screen_lines_command.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/editor_draw_3d_screen_lines_command.h"
							# terrain_cell_update_buffer
							"${CMAKE_CURRENT_SOURCE_DIR}/terrain_cell_update_buffer_command.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/terrain_cell_update_buffer_command.h"
							# terrain_exchange_texture
							"${CMAKE_CURRENT_SOURCE_DIR}/terrain_exchange_texture_command.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/terrain_exchange_texture_command.h"
							# engine_renderer
							"${CMAKE_CURRENT_SOURCE_DIR}/engine_renderer.cpp"
							# scene_renderer
							"${CMAKE_CURRENT_SOURCE_DIR}/scene_renderer.cpp"
							# commands
							# update_skeleton
							"${CMAKE_CURRENT_SOURCE_DIR}/update_skeleton_command.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/update_skeleton_command.h"
							# update_model_vertex_buffer
							"${CMAKE_CURRENT_SOURCE_DIR}/update_model_vertex_buffer_command.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/update_model_vertex_buffer_command.h"
							# ui_renderer
							"${CMAKE_CURRENT_SOURCE_DIR}/ui_renderer.cpp"
							# commands
							# draw_vertices
							"${CMAKE_CURRENT_SOURCE_DIR}/ui_draw_vertices_command.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/ui_draw_vertices_command.h"
							# vertex_input_type
							"${CMAKE_CURRENT_SOURCE_DIR}/vertex_input_type.cpp"
							# common commands
							# functor
							"${CMAKE_CURRENT_SOURCE_DIR}/functor_command.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/functor_command.h"
							# functor_with_big_buffer_to_copy
							"${CMAKE_CURRENT_SOURCE_DIR}/functor_with_big_buffer_to_copy_command.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/functor_with_big_buffer_to_copy_command_inline.h"
							# ./
							"${CMAKE_CURRENT_SOURCE_DIR}/render_facade_entry_point.cpp"
)

target_link_libraries(renderer_facade PRIVATE
	core
)

set_target_properties(renderer_facade PROPERTIES FOLDER "engine/renderers")
