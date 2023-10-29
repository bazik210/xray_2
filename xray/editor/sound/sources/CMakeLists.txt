project(sound_editor LANGUAGES CXX CSharp)

add_library(sound_editor SHARED
							# core
							"${CMAKE_CURRENT_SOURCE_DIR}/editor_creator.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/editor_creator.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_editor_library_linkage.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_editor_memory.h"
							# include
							"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
							# resources
							"${CMAKE_CURRENT_SOURCE_DIR}/resources.resx"
							# images
							"${CMAKE_CURRENT_SOURCE_DIR}/images/images16x16.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/images/images16x16.png"
							# sound_editor
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_editor.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_editor.h"
							# form
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_editor_form.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_editor_form.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_editor_form.resx"
							# view_controls
							"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_helper.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_helper.h"
							# document
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_document_editor_base.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_document_editor_base.h"
							# composite
							"${CMAKE_CURRENT_SOURCE_DIR}/composite_sound_document.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/composite_sound_document.h"
							# commands
							"${CMAKE_CURRENT_SOURCE_DIR}/composite_sound_commands.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/composite_sound_commands.h"
							# collection
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_collection_document.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_collection_document.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_collection_document.resx"
							# commands
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_collection_commands.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_collection_commands.h"
							# sound_scene
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_scene_document.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_scene_document.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_scene_document_components.cpp"
							# commands
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_scene_commands.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_scene_commands.h"
							# actions
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_scene_actions.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_scene_actions.h"
							# apply_revert_form
							"${CMAKE_CURRENT_SOURCE_DIR}/apply_revert_form.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/apply_revert_form.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/apply_revert_form.resx"
							# contexts_manager
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_editor_contexts_manager.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_editor_contexts_manager.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_scene_contexts_manager.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_scene_contexts_manager.h"
							# sound_object
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_object_wrapper.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_object_wrapper.h"
							# composite
							"${CMAKE_CURRENT_SOURCE_DIR}/composite_sound_wrapper.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/composite_sound_wrapper.h"
							# instance
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_object_instance.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_object_instance.h"
							# collision
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_object_collision.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_object_collision.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_object_collision_dynamic.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_object_collision_dynamic.h"
							# single_sound
							"${CMAKE_CURRENT_SOURCE_DIR}/single_sound_wrapper.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/single_sound_wrapper.h"
							# wav_file_options
							"${CMAKE_CURRENT_SOURCE_DIR}/wav_file_options.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/wav_file_options.h"
							# collection
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_collection_wrapper.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_collection_wrapper.h"
							# item
							"${CMAKE_CURRENT_SOURCE_DIR}/composite_sound_item.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/composite_sound_item.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_collection_item.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_collection_item.h"
							# panels
							# instance_properties
							"${CMAKE_CURRENT_SOURCE_DIR}/instance_properties_panel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/instance_properties_panel.h"
							# object_properties
							"${CMAKE_CURRENT_SOURCE_DIR}/object_properties_panel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/object_properties_panel.h"
							# toolbar
							"${CMAKE_CURRENT_SOURCE_DIR}/toolbar_panel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/toolbar_panel.h"
							# graph_view
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_view.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_view.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/graph_view.resx"
							# tree_view
							"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_files_source.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_files_source.h"
							"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_panel.cpp"
							"${CMAKE_CURRENT_SOURCE_DIR}/tree_view_panel.h"
							# ./
							"${CMAKE_CURRENT_SOURCE_DIR}/sound_editor_entry_point.cpp"
)

target_link_libraries(sound_editor PUBLIC
	engine_pc_dx11
)

set_property(TARGET sound_editor PROPERTY VS_GLOBAL_RootNamespace sound_editor)

set_target_properties(sound_editor PROPERTIES FOLDER "engine/editors")

add_dependencies(sound_editor editor_base editor_controls)
