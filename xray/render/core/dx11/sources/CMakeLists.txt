project(renderer_core_pc_dx11 LANGUAGES CXX)

add_library(renderer_core_pc_dx11 STATIC
									# core
									"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/static_type.cpp"
									# include
									"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../d3d_defines.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../dx10_defines.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../dx11_defines.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../render_include.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_include_getter.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../static_type.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../texture_options.h"
									# macros
									"${CMAKE_CURRENT_SOURCE_DIR}/../../macros_extensions.h"
									# resources
									"${CMAKE_CURRENT_SOURCE_DIR}/../render_target.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_common.h"
									# intrusive_base
									"${CMAKE_CURRENT_SOURCE_DIR}/../../resource_intrusive_base.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../resource_intrusive_base_inline.h"
									# buffers
									# index
									"${CMAKE_CURRENT_SOURCE_DIR}/../../index_buffer.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../index_buffer_inline.h"
									# vertex
									"${CMAKE_CURRENT_SOURCE_DIR}/../../vertex_buffer.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../vertex_buffer_inline.h"
									# untyped
									"${CMAKE_CURRENT_SOURCE_DIR}/../../untyped_buffer.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../untyped_buffer_inline.h"
									# shader constants
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_constant_storage.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../effect_constant_storage.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_defines.h"
									# slot
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_slot.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_slot_inline.h"
									# source
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_source.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_source_inline.h"
									# host
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_host.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_host_inline.h"
									# binding
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_binding.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_binding_inline.h"
									# constant
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_inline.h"
									# table
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_table.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_table_inline.h"
									# buffer
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_buffer.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_buffer_inline.h"
									# bindings
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_bindings.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_constant_bindings_inline.h"
									# destroy_data_helper
									"${CMAKE_CURRENT_SOURCE_DIR}/../../destroy_data_helper.h"
									# extensions
									# backend
									"${CMAKE_CURRENT_SOURCE_DIR}/backend.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../backend.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/backend_handlers.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../backend_handlers.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../backend_handlers_inline.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../backend_inline.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../blender_defines.h"
									# resources
									# manager
									"${CMAKE_CURRENT_SOURCE_DIR}/resource_manager.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../resource_manager.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../resource_manager_inline.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/shader_binary_source_cook.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/shader_binary_source_cook.h"
									# buffers
									# index
									"${CMAKE_CURRENT_SOURCE_DIR}/index_buffer.cpp"
									# vertex
									"${CMAKE_CURRENT_SOURCE_DIR}/vertex_buffer.cpp"
									# untyped
									"${CMAKE_CURRENT_SOURCE_DIR}/untyped_buffer.cpp"
									# samplers
									"${CMAKE_CURRENT_SOURCE_DIR}/res_sampler_list.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_sampler_list.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/res_sampler_state.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_sampler_state.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/sampler_slot.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../sampler_slot.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/sampler_state_descriptor.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../sampler_state_descriptor.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../sampler_state_descriptor_inline.h"
									# shaders
									"${CMAKE_CURRENT_SOURCE_DIR}/res_gs_hw.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_gs_hw.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/res_ps_hw.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_ps_hw.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/res_vs_hw.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_vs_hw.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/res_xs.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_xs.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_xs_hw.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_xs_hw_impl.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../shader_configuration.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/shader_declarated_macros.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_declarated_macros.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_defines.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/shader_macros.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../shader_macros.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../xs_data.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/xs_descriptor.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../xs_descriptor.h"
									# textures
									"${CMAKE_CURRENT_SOURCE_DIR}/res_texture.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_texture.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/res_texture_list.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_texture_list.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/texture_pool.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/texture_pool.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../texture_slot.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/texture_storage.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/texture_storage.h"
									# state
									"${CMAKE_CURRENT_SOURCE_DIR}/res_state.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_state.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/state_cache.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../state_cache.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../state_cache_inline.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/state_descriptor.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../state_descriptor.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/state_utils.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../state_utils.h"
									# geometry
									"${CMAKE_CURRENT_SOURCE_DIR}/res_declaration.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_declaration.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/res_geometry.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_geometry.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/res_input_layout.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_input_layout.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/res_signature.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../res_signature.h"
									# shader constants
									# table
									"${CMAKE_CURRENT_SOURCE_DIR}/shader_constant_table.cpp"
									# buffer
									"${CMAKE_CURRENT_SOURCE_DIR}/shader_constant_buffer.cpp"
									# render_target
									"${CMAKE_CURRENT_SOURCE_DIR}/render_target.cpp"
									# device
									"${CMAKE_CURRENT_SOURCE_DIR}/device.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../device.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/device_caps.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../device_caps.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/res_render_output.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../res_render_output.h"
									# effect_manager
									"${CMAKE_CURRENT_SOURCE_DIR}/custom_config.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../custom_config.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/custom_config_value.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../custom_config_value.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../custom_config_value_inline.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_compiler.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../effect_compiler.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_descriptor.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../effect_descriptor.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_manager.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../effect_manager.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../effect_manager_inline.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_options_descriptor.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../effect_options_descriptor.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/manager_common_inline.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/res_effect.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../res_effect.h"
									# options
									"${CMAKE_CURRENT_SOURCE_DIR}/options.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../options.h"
									# utilities
									"${CMAKE_CURRENT_SOURCE_DIR}/com_utils.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/dds.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/dds.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/decl_utils.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../decl_utils.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/pix_event_wrapper.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../pix_event_wrapper.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../quasi_singleton.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../utils.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../utils_inline.h"
									# gpu_timer
									"${CMAKE_CURRENT_SOURCE_DIR}/gpu_timer.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../gpu_timer.h"
									# effect_cook
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_cook.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../effect_cook.h"
									# texture
									"${CMAKE_CURRENT_SOURCE_DIR}/texture_cook_register.cpp"
									# converter_cook
									"${CMAKE_CURRENT_SOURCE_DIR}/texture_converter_cook.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/texture_converter_cook.h"
									# cook
									"${CMAKE_CURRENT_SOURCE_DIR}/texture_cook.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/texture_cook.h"
									# cook_wrapper
									"${CMAKE_CURRENT_SOURCE_DIR}/texture_cook_wrapper.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/texture_cook_wrapper.h"
									# options
									"${CMAKE_CURRENT_SOURCE_DIR}/texture_options.cpp"
									# ./
									"${CMAKE_CURRENT_SOURCE_DIR}/render_core_entry_point.cpp"
)

target_include_directories(renderer_core_pc_dx11 PUBLIC
	"${DXSDK_DIR_PATH}/Include"
)

target_link_directories(renderer_core_pc_dx11 PUBLIC
	"${DXSDK_DIR_PATH}/Lib/x64"
)

set_target_properties(renderer_core_pc_dx11 PROPERTIES FOLDER "engine/renderers/pc/dx11")
