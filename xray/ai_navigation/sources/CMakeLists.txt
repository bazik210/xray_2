project(ai_navigation LANGUAGES CXX)

add_library(ai_navigation STATIC
							# core
							"${CMAKE_CURRENT_SOURCE_DIR}/ai_navigation_entry_point.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/ai_navigation_library_linkage.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/ai_navigation_memory.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
							# graph_generator
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_generate.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_path_find_test.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_predicates.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_restricted_area_cleaner.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_serializer.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_test_playground.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_triangles_intersection.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_triangles_intersection.h"
							# merger
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_merger.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_merger.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_merger_constraint_collector.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_merger_constraint_subdivider.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_merger_duplicates_remover.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_merger_remove_similar_edges.cpp"
							# fuser
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_fuser.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_fuser.h"
							# adjacency_builder
							"${CMAKE_CURRENT_SOURCE_DIR}/adjacency_builder_inline.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_adjacency_builder.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_adjacency_builder.h"
							# t_junction_remover
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_t_junction_remover.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_t_junction_remover.h"
							# tessellator
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_tessellator.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_tessellator.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_tessellator_coplanar_case.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_tessellator_degenerated_cases.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_tessellator_generic_case.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_tessellator_triangle_subtraction.cpp"
							# extruder
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_extruder.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_extruder.h"
							# disconnected_regions_remover
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_disconnected_regions_remover.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_disconnected_regions_remover.h"
							# subdivider
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_subdivider.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_generator_subdivider.h"
							# delaunay_triangulator
							"${CMAKE_CURRENT_SOURCE_DIR}/constrained_delaunay_triangulator.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/constrained_delaunay_triangulator.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/constrained_delaunay_triangulator_inline.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/delaunay_debug_functions.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/delaunay_triangulator.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/delaunay_triangulator.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/delaunay_triangulator_inline.h"
							# path_finder
							"${CMAKE_CURRENT_SOURCE_DIR}/path_finder_channel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/path_finder_channel.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/path_finder_funnel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/path_finder_funnel.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/path_finder_modified_funnel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/path_finder_modified_funnel.h"
							# world
							"${CMAKE_CURRENT_SOURCE_DIR}/navigation_world.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/navigation_world.h"
							# navigation_mesh
							"${CMAKE_CURRENT_SOURCE_DIR}/navigation_mesh_functions.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/navigation_mesh_functions.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/navigation_mesh_types.h"
							# triangles_mesh
							"${CMAKE_CURRENT_SOURCE_DIR}/triangles_mesh.h"
							# include
							"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
							#"${CMAKE_CURRENT_SOURCE_DIR}/math.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
							# ./
)

target_link_libraries(ai_navigation PUBLIC
	core
)

set_target_properties(ai_navigation PROPERTIES FOLDER "engine/ai")
