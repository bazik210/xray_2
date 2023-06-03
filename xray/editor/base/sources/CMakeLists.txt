project(editor_base LANGUAGES CXX CSharp)

add_library(editor_base SHARED
					# core
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_base_library_linkage.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_base_memory.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_creator.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_creator.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/make_public_helper.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_holder_delegates_types.h"
					# include
					"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../converting.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_base.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../editor_extensions.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../editor_intrusive_base.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../editor_intrusive_base_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../managed_delegate.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../unmanaged_string.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../unmanaged_string_inline.h"
					# properties
					"${CMAKE_CURRENT_SOURCE_DIR}/lua_converted_property_value.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/lua_converted_property_value.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/lua_property_value.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/lua_property_value.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_properties.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/object_properties.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_value_include.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/value_attribute.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/value_attribute.h"
					# values
					# boolean
					# value
					# value
					"${CMAKE_CURRENT_SOURCE_DIR}/property_boolean.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_boolean.h"
					# reference
					"${CMAKE_CURRENT_SOURCE_DIR}/property_boolean_reference.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_boolean_reference.h"
					# values
					# reference
					"${CMAKE_CURRENT_SOURCE_DIR}/property_boolean_values_value_reference.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_boolean_values_value_reference.h"
					# value
					"${CMAKE_CURRENT_SOURCE_DIR}/property_boolean_values_value.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_boolean_values_value.h"
					# color
					# value
					"${CMAKE_CURRENT_SOURCE_DIR}/property_color.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_color.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_system_color.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_system_color.h"
					# reference
					"${CMAKE_CURRENT_SOURCE_DIR}/property_color_reference.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_color_reference.h"
					# float
					# values_enum
					# value
					"${CMAKE_CURRENT_SOURCE_DIR}/property_float_enum_value.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_float_enum_value.h"
					# reference
					"${CMAKE_CURRENT_SOURCE_DIR}/property_float_enum_value_reference.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_float_enum_value_reference.h"
					# value_limited
					# value
					"${CMAKE_CURRENT_SOURCE_DIR}/property_float_limited.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_float_limited.h"
					# reference
					"${CMAKE_CURRENT_SOURCE_DIR}/property_float_limited_reference.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_float_limited_reference.h"
					# value
					# reference
					"${CMAKE_CURRENT_SOURCE_DIR}/property_float.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_float.h"
					# value
					"${CMAKE_CURRENT_SOURCE_DIR}/property_float_reference.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_float_reference.h"
					# integer
					# values_enum
					# value
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_enum_value.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_enum_value.h"
					# reference
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_enum_value_reference.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_enum_value_reference.h"
					# values
					# base
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_values_value_base.h"
					# value_getter
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_values_value_getter.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_values_value_getter.h"
					# value
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_values_value.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_values_value.h"
					# reference_getter
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_values_value_reference_getter.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_values_value_reference_getter.h"
					# reference
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_values_value_reference.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_values_value_reference.h"
					# value_limited
					# value
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_limited.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_limited.h"
					# reference
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_limited_reference.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_limited_reference.h"
					# value
					# value
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer.h"
					# reference
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_reference.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_integer_reference.h"
					# string
					# values
					# base
					"${CMAKE_CURRENT_SOURCE_DIR}/property_string_values_value_base.h"
					# value_getter
					"${CMAKE_CURRENT_SOURCE_DIR}/property_string_values_value_getter.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_string_values_value_getter.h"
					# value
					"${CMAKE_CURRENT_SOURCE_DIR}/property_string_values_value.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_string_values_value.h"
					# shared_getter
					"${CMAKE_CURRENT_SOURCE_DIR}/property_string_values_value_shared_str_getter.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_string_values_value_shared_str_getter.h"
					# shared
					"${CMAKE_CURRENT_SOURCE_DIR}/property_string_values_value_shared_str.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_string_values_value_shared_str.h"
					# value
					# value
					"${CMAKE_CURRENT_SOURCE_DIR}/property_string.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_string.h"
					# shared
					"${CMAKE_CURRENT_SOURCE_DIR}/property_string_shared_str.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_string_shared_str.h"
					# vec3f
					# value
					"${CMAKE_CURRENT_SOURCE_DIR}/property_vec3f.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_vec3f.h"
					# reference
					"${CMAKE_CURRENT_SOURCE_DIR}/property_vec3f_reference.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_vec3f_reference.h"
					# converters
					"${CMAKE_CURRENT_SOURCE_DIR}/Float3_to_Vector3D_converter.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/Float3_to_Vector3D_converter.h"
					# command_engine
					"${CMAKE_CURRENT_SOURCE_DIR}/command.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_engine.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_engine.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_engine_inline.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_transaction.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/command_transaction.h"
					# resources
					"${CMAKE_CURRENT_SOURCE_DIR}/resources.resx"
					# images
					"${CMAKE_CURRENT_SOURCE_DIR}/../images/images16x16.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../images/images16x16.png"
					"${CMAKE_CURRENT_SOURCE_DIR}/../images/images96x32.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../images/images96x32.png"
					# tree_values
					"${CMAKE_CURRENT_SOURCE_DIR}/window_tree_values.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/window_tree_values.h"
					# input_engine
					"${CMAKE_CURRENT_SOURCE_DIR}/input_engine.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/input_engine.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/input_engine_process.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/input_engine_serialize.cpp"
					# gui_binder
					"${CMAKE_CURRENT_SOURCE_DIR}/gui_binder.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/gui_binder.h"
					# action
					"${CMAKE_CURRENT_SOURCE_DIR}/action.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/action.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/action_continuous.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/action_shortcut.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/action_shortcut.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/action_single.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/actions_layer.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/actions_layer.h"
					# input_keys_holder
					"${CMAKE_CURRENT_SOURCE_DIR}/input_keys_holder.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/input_keys_holder.h"
					# action_delegate
					"${CMAKE_CURRENT_SOURCE_DIR}/action_delegate.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/action_delegate.h"
					# i_context_manager
					"${CMAKE_CURRENT_SOURCE_DIR}/../i_context_manager.h"
					# options
					# options_dialog
					"${CMAKE_CURRENT_SOURCE_DIR}/options_dialog.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/options_dialog.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/options_dialog.resx"
					# option_pages
					"${CMAKE_CURRENT_SOURCE_DIR}/options_page.h"
					# options_input_page
					"${CMAKE_CURRENT_SOURCE_DIR}/options_page_input.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/options_page_input.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/options_page_input.resx"
					# wpf_controls_page
					"${CMAKE_CURRENT_SOURCE_DIR}/wpf_controls_page.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/wpf_controls_page.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/wpf_controls_page.resx"
					# view_panel
					# editor controls
					"${CMAKE_CURRENT_SOURCE_DIR}/collision_object_control.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/../collision_object_control.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../collision_object_types.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_control_base.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_control_base.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_actions.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_actions.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_base.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_base.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/../transform_control_defines.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_object.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_rotation.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_rotation.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_rotation_draw.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_scaling.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_scaling.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_scaling_draw.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_translation.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_translation.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_translation_draw.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_translation_snap.cpp"
					# transform_control_objects
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_objects_impl.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/transform_control_objects_impl.h"
					# property_visualizers
					"${CMAKE_CURRENT_SOURCE_DIR}/property_visualizers.h"
					# view control
					"${CMAKE_CURRENT_SOURCE_DIR}/camera_view_panel.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/camera_view_panel.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/scene_view_panel.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/scene_view_panel.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/scene_view_panel.resx"
					"${CMAKE_CURRENT_SOURCE_DIR}/scene_view_panel_light.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/view_control_actions.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/view_control_actions.h"
					# helpers
					"${CMAKE_CURRENT_SOURCE_DIR}/editors_helper.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/editors_helper.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/fs_helper.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/fs_helper.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/operators.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/operators.h"
					# resource_chooser
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_chooser.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_chooser.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_chooser_external_editor.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/resource_chooser_external_editor.h"
					# properties_holder
					"${CMAKE_CURRENT_SOURCE_DIR}/properties_holder.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/properties_holder.h"
					# property_value_types
					"${CMAKE_CURRENT_SOURCE_DIR}/property_value_base.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_value_bool.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_value_color.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_value_editable_collection.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_value_editable_collection.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_value_float.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_value_float3.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_value_integer.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_value_lists_dictionary.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_value_lists_dictionary.h"
					"${CMAKE_CURRENT_SOURCE_DIR}/property_value_string.h"
					# curves_serializer
					"${CMAKE_CURRENT_SOURCE_DIR}/curves_serializer.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/curves_serializer.h"
					# ./
					"${CMAKE_CURRENT_SOURCE_DIR}/editor_base_entry_point.cpp"
					"${CMAKE_CURRENT_SOURCE_DIR}/images_loading.h"
					#TODO MISSING
					#"${CMAKE_CURRENT_SOURCE_DIR}/resource.h"
)

target_link_libraries(editor_base PUBLIC
	engine_pc_dx11
	wpf_controls
)

target_compile_options(editor_base PRIVATE
    /AI\"${CMAKE_LIBRARY_OUTPUT_DIRECTORY}\"
)
set_target_properties(editor_base PROPERTIES DOTNET_TARGET_FRAMEWORK_VERSION "v3.5")
set_target_properties(editor_base PROPERTIES VS_DOTNET_REFERENCES
    "PresentationCore;PresentationFramework;System;System.Core;System.Data;System.Design;System.Drawing;System.Windows.Forms;System.Xml;WeifenLuo.WinForms.UI.Docking;WindowsBase;wpf_controls;WPFToolkit.Extended;xray_editor_controls;xray_engine_pc_dx11"
)

set_target_properties(editor_base PROPERTIES FOLDER "engine/editors")
