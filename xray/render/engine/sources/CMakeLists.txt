project(renderers_dx11 LANGUAGES CXX)

add_library(renderer_engine_pc_dx11 STATIC
									# core
									"${CMAKE_CURRENT_SOURCE_DIR}/geometry_type.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/render_engine_library_linkage.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/shared_names.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/vertex_formats.h"
									# convex_volume
									"${CMAKE_CURRENT_SOURCE_DIR}/convex_volume.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/convex_volume.h"
									# sphere_geometry
									"${CMAKE_CURRENT_SOURCE_DIR}/du_sphere.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/du_sphere.h"
									# polymorph_vector
									"${CMAKE_CURRENT_SOURCE_DIR}/polymorph_vector.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/polymorph_vector_inline.h"
									# resources
									# model
									"${CMAKE_CURRENT_SOURCE_DIR}/collision_object.h"
									# converter
									"${CMAKE_CURRENT_SOURCE_DIR}/model_converter.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/model_converter.h"
									# model_cook
									"${CMAKE_CURRENT_SOURCE_DIR}/model_cooker.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/model_cooker.h"
									# manager
									"${CMAKE_CURRENT_SOURCE_DIR}/model_manager.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/model_manager.h"
									# render_models
									# skeleton
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model_skeleton.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model_skeleton.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_mesh_cpu_skinning_4weights.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_mesh_cpu_skinning_4weights.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_mesh_gpu_skinning_4weights.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_mesh_gpu_skinning_4weights.h"
									# base
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model.h"
									# cook
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model_cooker.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model_cooker.h"
									# static
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model_static.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model_static.h"
									# user
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model_user.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model_user.h"
									# terrain
									"${CMAKE_CURRENT_SOURCE_DIR}/terrain_render_model.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/terrain_render_model.h"
									# composite
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model_composite.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model_composite.h"
									# grass
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model_grass.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/render_model_grass.h"
									# material
									"${CMAKE_CURRENT_SOURCE_DIR}/material.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/material.h"
									# cook
									"${CMAKE_CURRENT_SOURCE_DIR}/material_cook.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/material_cook.h"
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/material_effects.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/material_effects.h"
									# cook
									"${CMAKE_CURRENT_SOURCE_DIR}/material_effects_instance_cook.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/material_effects_instance_cook.h"
									# manager
									"${CMAKE_CURRENT_SOURCE_DIR}/material_manager.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/material_manager.h"
									# effect_material_base
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_material_base.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_material_base.h"
									# terrain
									"${CMAKE_CURRENT_SOURCE_DIR}/terrain.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/terrain.h"
									# cook
									"${CMAKE_CURRENT_SOURCE_DIR}/terrain_cook.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/terrain_cook.h"
									# texture_pool
									"${CMAKE_CURRENT_SOURCE_DIR}/terrain_texture_pool.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/terrain_texture_pool.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/terrain_texture_pool_inline.h"
									# particle_emitter_instance
									"${CMAKE_CURRENT_SOURCE_DIR}/render_particle_emitter_instance.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/render_particle_emitter_instance.h"
									# render_output_window
									"${CMAKE_CURRENT_SOURCE_DIR}/render_output_window.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/render_output_window.h"
									# cook
									"${CMAKE_CURRENT_SOURCE_DIR}/render_output_window_cook.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/render_output_window_cook.h"
									# scene
									"${CMAKE_CURRENT_SOURCE_DIR}/scene.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/scene.h"
									# manager
									"${CMAKE_CURRENT_SOURCE_DIR}/scene_manager.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/scene_manager.h"
									# view
									"${CMAKE_CURRENT_SOURCE_DIR}/scene_view.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/scene_view.h"
									# cook
									"${CMAKE_CURRENT_SOURCE_DIR}/scene_view_cook.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/scene_view_cook.h"
									# camera
									"${CMAKE_CURRENT_SOURCE_DIR}/camera.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/camera_inline.h"
									# cook
									"${CMAKE_CURRENT_SOURCE_DIR}/scene_cook.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/scene_cook.h"
									# animated_model_instance
									# cook
									"${CMAKE_CURRENT_SOURCE_DIR}/animated_model_instance_cook.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/animated_model_instance_cook.h"
									# speedtree
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree.h"
									# tree
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_tree.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_tree.h"
									# branch
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_tree_component_branch.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_tree_component_branch.h"
									# frond
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_tree_component_frond.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_tree_component_frond.h"
									# leafmesh
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_tree_component_leafmesh.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_tree_component_leafmesh.h"
									# leafcard
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_tree_component_leafcard.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_tree_component_leafcard.h"
									# billboard
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_tree_component_billboard.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_tree_component_billboard.h"
									# instance_impl
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_instance_impl.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_instance_impl.h"
									# cook
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_cook.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_cook.h"
									# convert_type
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_convert_type.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_convert_type.h"
									# parameters
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_shader_parameters.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_shader_parameters.h"
									# forest
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_forest.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/speedtree_forest.h"
									# grass
									"${CMAKE_CURRENT_SOURCE_DIR}/grass.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/grass.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/grass_instance.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/grass_instance.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/grass_patch.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/grass_patch.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/grass_template.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/grass_template.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/grass_world.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/grass_world.h"
									# include
									"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../base_classes.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../model_format.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../vertex_colored.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../world_pc.h"
									# world
									# options
									"${CMAKE_CURRENT_SOURCE_DIR}/engine_options.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/engine_options.h"
									# renderer
									"${CMAKE_CURRENT_SOURCE_DIR}/register_effect_descriptors.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/renderer.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/renderer_register_effects.cpp"
									# stages
									"${CMAKE_CURRENT_SOURCE_DIR}/blend_mode.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage.h"
									# lighting
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_lights.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_lights.h"
									# materials
									# default
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_lighting_stage_default_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_lighting_stage_default_materials.h"
									# shadow
									"${CMAKE_CURRENT_SOURCE_DIR}/depth_accumulate_material_effect.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/depth_accumulate_material_effect.h"
									# organic
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_downsample_skin_irradiance_texture.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_downsample_skin_irradiance_texture.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fix_irradiance_texture.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fix_irradiance_texture.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/organic_lighting_passes.h"
									# human skin
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_lighting_stage_skin_base_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_lighting_stage_skin_base_materials.h"
									# body
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_lighting_stage_organic_base_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_lighting_stage_organic_base_materials.h"
									# light
									"${CMAKE_CURRENT_SOURCE_DIR}/light.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/light.h"
									# effects
									# capsule
									"${CMAKE_CURRENT_SOURCE_DIR}/capsule_light_effect.h"
									# obb
									"${CMAKE_CURRENT_SOURCE_DIR}/obb_light_effect.h"
									# parallel
									"${CMAKE_CURRENT_SOURCE_DIR}/parallel_light_effect.h"
									# point
									"${CMAKE_CURRENT_SOURCE_DIR}/point_light_effect.h"
									# spot
									"${CMAKE_CURRENT_SOURCE_DIR}/spot_light_effect.h"
									# sphere
									"${CMAKE_CURRENT_SOURCE_DIR}/sphere_light_effect.h"
									# plane_spot
									"${CMAKE_CURRENT_SOURCE_DIR}/plane_spot_light_effect.h"
									# db
									"${CMAKE_CURRENT_SOURCE_DIR}/lights_db.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/lights_db.h"
									# forward
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_forward.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_forward.h"
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_blend_subuv_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_blend_subuv_materials.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_default_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_default_materials.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_fire_fresnel_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_fire_fresnel_materials.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_smoke_custom_lighted_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_smoke_custom_lighted_materials.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_soft_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_soft_materials.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_volume_cone_base_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_volume_cone_base_materials.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_volume_sphere_base_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fstage_volume_sphere_base_materials.h"
									# g_stage
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_gbuffer.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_gbuffer.h"
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gbuffer_nomaterial_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gbuffer_nomaterial_materials.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_terrain_new.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_terrain_new.h"
									# materials
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gstage_burning_wood_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gstage_burning_wood_materials.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gstage_default_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gstage_default_materials.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gstage_terrain_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gstage_terrain_materials.h"
									# post_process
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_postprocess.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_postprocess.h"
									# distortion
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_accumulate_distortion.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_accumulate_distortion.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_apply_distortion.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_apply_distortion.h"
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_apply_distortion.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_apply_distortion.h"
									# materials
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_distortion_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_distortion_materials.h"
									# default
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_distortion_stage_default_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_distortion_stage_default_materials.h"
									# panner
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_distortion_stage_panner_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_distortion_stage_panner_materials.h"
									# blur
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_blur.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_blur.h"
									# blend
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_complex_post_process_blend.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_complex_post_process_blend.h"
									# materials
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_blend_texture_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_blend_texture_materials.h"
									# ssao
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_ssao_accumulation.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_ssao_accumulation.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_ssao_filter4x4.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_ssao_filter4x4.h"
									# copy_image
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_copy_image.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_copy_image.h"
									# parameters
									"${CMAKE_CURRENT_SOURCE_DIR}/post_process_parameters.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/post_process_parameters.h"
									# multisample anti-aliasing
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_antialiasing.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_antialiasing.h"
									# bloom
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gather_bloom.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gather_bloom.h"
									# luminance
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gather_luminance.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gather_luminance.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gather_luminance_histogram.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gather_luminance_histogram.h"
									# adaptation
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_eye_adaptation.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_eye_adaptation.h"
									# light_scattering
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gather_sun_light_scattering_zone.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_gather_sun_light_scattering_zone.h"
									# deferred_transparency
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_deferred_transparency.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_deferred_transparency.h"
									# mlaa
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_mlaa.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_mlaa.h"
									# fxaa
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_fxaa.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_fxaa.h"
									# system
									"${CMAKE_CURRENT_SOURCE_DIR}/aabb_indices.h"
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_model_ghost.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_model_ghost.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_forward_system.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_forward_system.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_terrain_debug_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_post_process_terrain_debug_materials.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_system_colored.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_system_colored.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_system_line.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_system_line.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_system_ui.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_system_ui.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_wireframe_colored.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_wireframe_colored.h"
									# fresnel_test
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_fr_test.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_fr_test.h"
									# sky_box
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_skybox.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_skybox.h"
									# material
									"${CMAKE_CURRENT_SOURCE_DIR}/sky_default_material_effect.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/sky_default_material_effect.h"
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_geometry_complexity.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_geometry_complexity.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_selection.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_selection.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_show_miplevel.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_show_miplevel.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_particle_selection.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_particle_selection.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_speedtree_selection.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_speedtree_selection.h"
									# view_mode
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_view_mode.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_view_mode.h"
									# effects
									# overdraw
									# accumulate
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_accumulate_overdraw.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_accumulate_overdraw.h"
									# show
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_show_overdraw.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_show_overdraw.h"
									# texture_density
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_texture_density.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_texture_density.h"
									# wireframe
									# accumulate
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_wireframe_accumulation.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_wireframe_accumulation.h"
									# apply
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_apply_wireframe.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_apply_wireframe.h"
									# gbuffer_to_screen
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_gbuffer_to_screen.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_gbuffer_to_screen.h"
									# shader_complexity
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_shader_complexity.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_editor_shader_complexity.h"
									# screen_image
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_screen_image.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_screen_image.h"
									# debug
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_debug.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_debug.h"
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_debug_editor_wireframe.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_debug_editor_wireframe.h"
									# sun
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_sun.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_sun.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/sun_cascade.h"
									# pre_lighting
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_pre_lighting.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_pre_lighting.h"
									# sun_materials
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_sun_materials.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_sun_materials.h"
									# shadow_map_direct
									"${CMAKE_CURRENT_SOURCE_DIR}/ray.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/shadow_cascade_volume.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_shadow_direct.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_shadow_direct.h"
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_light_direct.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_light_mask.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_light_mask.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_shadow_map.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_shadow_map.h"
									# decals_accumulate
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_decals_accumulate.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_decals_accumulate.h"
									# instance
									"${CMAKE_CURRENT_SOURCE_DIR}/decal_instance.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/decal_instance.h"
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/decal_default_material_effect.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/decal_default_material_effect.h"
									# mask
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_decal_mask.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_decal_mask.h"
									# ambient_occlusion
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_ambient_occlusion.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_ambient_occlusion.h"
									# light_propagation_volumes
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_light_propagation_volumes.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_light_propagation_volumes.h"
									# effects
									# default_fill
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fill_reflective_shadow_map.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_fill_reflective_shadow_map.h"
									# downsample_rsm
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_downsample_reflective_shadow_map.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_downsample_reflective_shadow_map.h"
									# apply_indirect_lighting
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_apply_indirect_lighting.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_apply_indirect_lighting.h"
									# downsample_gbuffer
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_downsample_gbuffer.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_downsample_gbuffer.h"
									# rediance_volume
									"${CMAKE_CURRENT_SOURCE_DIR}/radiance_volume.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/radiance_volume.h"
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_light_propagation_volumes.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_light_propagation_volumes.h"
									# inject_lighting
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_radiance_volume_inject_lighting.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_radiance_volume_inject_lighting.h"
									# propagate_lighting
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_radiance_volume_propagate_lighting.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_radiance_volume_propagate_lighting.h"
									# clouds
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_clouds.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_clouds.h"
									# effects
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_clouds.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/effect_clouds.h"
									# system
									"${CMAKE_CURRENT_SOURCE_DIR}/system_renderer.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/system_renderer.h"
									# context
									"${CMAKE_CURRENT_SOURCE_DIR}/renderer_context.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/renderer_context.h"
									# context_targets
									"${CMAKE_CURRENT_SOURCE_DIR}/renderer_context_targets.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/renderer_context_targets.h"
									# environment
									"${CMAKE_CURRENT_SOURCE_DIR}/environment.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/environment.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/moon_position.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/moon_position.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/sun_position.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/sun_position.h"
									# obsolete
									# particles
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_particles.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/stage_particles.h"
									# statistics
									"${CMAKE_CURRENT_SOURCE_DIR}/statistics.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/statistics.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/statistics_inline.h"
									# flash_renderer
									"${CMAKE_CURRENT_SOURCE_DIR}/flash_renderer.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/flash_renderer.h"
									# hal
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_BinaryShaders.inl"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_Config.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_HAL.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_HAL.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_HALSetup.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_MeshCache.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_MeshCache.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_Shader.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_Shader.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_ShaderManager.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_ShaderManager.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_Shaders.h"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_Shaders.inl"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_Texture.cpp"
									"${CMAKE_CURRENT_SOURCE_DIR}/../../../../scaleform/Src/Render/D3D1x/D3D1x_Texture.h"
									# ./
									"${CMAKE_CURRENT_SOURCE_DIR}/render_engine_world_pc_dx11.cpp"
)

set_target_properties(renderer_engine_pc_dx11 PROPERTIES FOLDER "engine/renderers/pc/dx11")



######################



add_library(renderer_engine_pc_opengl STATIC
										# core
										"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
										"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
										"${CMAKE_CURRENT_SOURCE_DIR}/render_engine_library_linkage.cpp"
										# include
										"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
										"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
										"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
										"${CMAKE_CURRENT_SOURCE_DIR}/../world_pc.h"
										# world
										"${CMAKE_CURRENT_SOURCE_DIR}/render_engine_world_pc_opengl.cpp"
										# renderer
										# stages
										# post_process
										# parameters
										"${CMAKE_CURRENT_SOURCE_DIR}/post_process_parameters.cpp"
										# scene
										# view
										"${CMAKE_CURRENT_SOURCE_DIR}/scene_view.cpp"
										# ./
)

set_target_properties(renderer_engine_pc_opengl PROPERTIES FOLDER "engine/renderers/pc/opengl")
