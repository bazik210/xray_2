project(particle LANGUAGES CXX)

add_library(particle STATIC
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_entity_types.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_library_linkage.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						# half_type
						"${CMAKE_CURRENT_SOURCE_DIR}/half.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/half_inline.h"
						# curve_line
						"${CMAKE_CURRENT_SOURCE_DIR}/curve_line.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/curve_line.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/curve_line_inline.h"
						# help_functions
						"${CMAKE_CURRENT_SOURCE_DIR}/help_functions.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/help_functions.h"
						# color_matrix
						"${CMAKE_CURRENT_SOURCE_DIR}/color_matrix.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/color_matrix.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/color_matrix_inline.h"
						# evaluate_type
						"${CMAKE_CURRENT_SOURCE_DIR}/evaluate_type.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/evaluate_type.h"
						# world
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_world.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_world.h"
						# cook
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_world_cooker.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_world_cooker.h"
						# include
						"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../base_particle.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../base_particle_inline.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../particle_data_type.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../render_particle_emitter_instance.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
						# particle_system
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_system.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_system.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_system_inline.h"
						# cook
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_system_cook.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_system_wrapper_cook.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_system_wrapper_cook.h"
						# instance
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_system_instance.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_system_instance.h"
						# cook
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_system_instance_cook.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_system_instance_cook.h"
						# impl
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_system_instance_impl.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_system_instance_impl.h"
						# actions
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_actions.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_actions.h"
						# domains
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_domains.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_domains.h"
						# preview_particle_system
						"${CMAKE_CURRENT_SOURCE_DIR}/preview_particle_system.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/preview_particle_system.h"
						# cook
						"${CMAKE_CURRENT_SOURCE_DIR}/preview_particle_system_cooker.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/preview_particle_system_cooker.h"
						# instance
						"${CMAKE_CURRENT_SOURCE_DIR}/preview_particle_system_instance.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/preview_particle_system_instance.h"
						# emitters
						# emitter
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_emitter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_emitter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_emitter_inline.h"
						# instance
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_emitter_instance.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_emitter_instance.h"
						# beam_emitter
						# instance
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_beam_emitter_instance.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_beam_emitter_instance.h"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/particle_entry_point.cpp"
)

target_link_libraries(particle PUBLIC
	core
)

set_target_properties(particle PROPERTIES FOLDER "engine")
