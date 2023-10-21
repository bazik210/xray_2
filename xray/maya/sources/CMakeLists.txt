project(maya LANGUAGES CXX)

add_library(maya2011 SHARED
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_library_linkage.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_memory.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/node_identifiers.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						# maya_engine
						# physics
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_body.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_body.h"
						# commands
						# create_body
						"${CMAKE_CURRENT_SOURCE_DIR}/create_body_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_body_cmd.h"
						# create_collision
						"${CMAKE_CURRENT_SOURCE_DIR}/create_collision_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_collision_cmd.h"
						# create_joint
						"${CMAKE_CURRENT_SOURCE_DIR}/create_joint_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_joint_cmd.h"
						# build_collision
						"${CMAKE_CURRENT_SOURCE_DIR}/build_collision_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/build_collision_cmd.h"
						# nodes
						"${CMAKE_CURRENT_SOURCE_DIR}/fake_drawing_locator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/fake_drawing_locator.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/geom_sources.cpp"
						# collision
						"${CMAKE_CURRENT_SOURCE_DIR}/collision_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/collision_node.h"
						# body
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node_manip.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node_manip.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/transform_drawable.h"
						# joint
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_node.h"
						# exporter
						"${CMAKE_CURRENT_SOURCE_DIR}/node_exportable.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/node_exportable.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/tmp.cpp"
						# utils
						"${CMAKE_CURRENT_SOURCE_DIR}/export_defines.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/helpers.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/helpers.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/RenderRoutines.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/RenderRoutines.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_path_commands.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_path_commands.h"
						# 3rd_party
						# MeshMender
						"${CMAKE_CURRENT_SOURCE_DIR}/NvMender2003/NVMeshMender.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/NvMender2003/NVMeshMender.h"
						# Tootle
						"${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/amd/tootle/tootlelib.h"
						# maya_engine
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_engine.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_engine.h"
						# export
						# skinned
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_bone_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_geom_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_serialize.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skinned_surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skinned_surface.h"
						# static
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_collector.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_collector.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_writer_geom_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_writer_serialize.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/static_surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/static_surface.h"
						# base
						"${CMAKE_CURRENT_SOURCE_DIR}/model_collector.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/model_collector.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/surface.h"
						# animation
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_export_util.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_export_util.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/export_utils.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/export_utils.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_export_util.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_export_util.h"
						# old_unused
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_text_file_export.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_text_file_export.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_assemble.cpp"
						# shading
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material_hw.cpp"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_entry_point.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/tools/maya_menu/xray_brdf.cgfx"
)

target_link_libraries(maya2011 PRIVATE
	vfs
	physics
	animation
	renderer_facade
	renderer_engine_pc_dx11
	particle
	collision
	LibFoundation
	LibPNG
#	scaleform
	bullet
	opengl32.lib
	OpenMayaUI.lib
	OpenMayaAnim.lib
	OpenMayaRender.lib
#	TootleSoftwareOnlyStatic_2k8_MTDLL64.lib
)


target_link_directories(maya2011 PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/xray/maya/maya2011_lib_x64
#	${SOLUTION_DIR_INCLUDE_PATH}/third_party/amd_tootle-master
)

set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp PROPERTY VS_SETTINGS "PrecompiledHeader=Create")

#target_precompile_headers(maya2011 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

set_target_properties(maya2011 PROPERTIES FOLDER "engine/plugins")

#########################################

add_library(maya2012 SHARED
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_library_linkage.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_memory.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/node_identifiers.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						# maya_engine
						# physics
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_body.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_body.h"
						# commands
						# create_body
						"${CMAKE_CURRENT_SOURCE_DIR}/create_body_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_body_cmd.h"
						# create_collision
						"${CMAKE_CURRENT_SOURCE_DIR}/create_collision_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_collision_cmd.h"
						# create_joint
						"${CMAKE_CURRENT_SOURCE_DIR}/create_joint_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_joint_cmd.h"
						# build_collision
						"${CMAKE_CURRENT_SOURCE_DIR}/build_collision_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/build_collision_cmd.h"
						# nodes
						"${CMAKE_CURRENT_SOURCE_DIR}/fake_drawing_locator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/fake_drawing_locator.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/geom_sources.cpp"
						# collision
						"${CMAKE_CURRENT_SOURCE_DIR}/collision_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/collision_node.h"
						# body
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node_manip.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node_manip.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/transform_drawable.h"
						# joint
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_node.h"
						# exporter
						"${CMAKE_CURRENT_SOURCE_DIR}/node_exportable.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/node_exportable.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/tmp.cpp"
						# utils
						"${CMAKE_CURRENT_SOURCE_DIR}/export_defines.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/helpers.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/helpers.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/RenderRoutines.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/RenderRoutines.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_path_commands.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_path_commands.h"
						# 3rd_party
						# MeshMender
						"${CMAKE_CURRENT_SOURCE_DIR}/NvMender2003/NVMeshMender.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/NvMender2003/NVMeshMender.h"
						# Tootle
						"${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/amd/tootle/tootlelib.h"
						# maya_engine
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_engine.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_engine.h"
						# export
						# skinned
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_bone_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_geom_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_serialize.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skinned_surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skinned_surface.h"
						# static
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_collector.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_collector.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_writer_geom_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_writer_serialize.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/static_surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/static_surface.h"
						# base
						"${CMAKE_CURRENT_SOURCE_DIR}/model_collector.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/model_collector.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/surface.h"
						# animation
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_export_util.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_export_util.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/export_utils.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/export_utils.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_export_util.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_export_util.h"
						# old_unused
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_text_file_export.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_text_file_export.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_assemble.cpp"
						# shading
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material_hw.cpp"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_entry_point.cpp"
)

target_link_libraries(maya2012 PRIVATE
	vfs
	physics
	animation
	renderer_facade
	renderer_engine_pc_dx11
	particle
	collision
	LibFoundation
	LibPNG
#	scaleform
	bullet
	opengl32.lib
	OpenMayaUI.lib
	OpenMayaAnim.lib
	OpenMayaRender.lib
#	TootleSoftwareOnlyStatic_2k8_MTDLL64.lib
)

target_link_directories(maya2012 PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/xray/maya/maya2012_lib_x64
#	${SOLUTION_DIR_INCLUDE_PATH}/third_party/amd_tootle-master
)

set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp PROPERTY VS_SETTINGS "PrecompiledHeader=Create")

#target_precompile_headers(maya2012 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

set_target_properties(maya2012 PROPERTIES FOLDER "engine/plugins")


#########################################

add_library(maya2018 SHARED
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_library_linkage.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_memory.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/node_identifiers.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						# maya_engine
						# physics
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_body.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_body.h"
						# commands
						# create_body
						"${CMAKE_CURRENT_SOURCE_DIR}/create_body_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_body_cmd.h"
						# create_collision
						"${CMAKE_CURRENT_SOURCE_DIR}/create_collision_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_collision_cmd.h"
						# create_joint
						"${CMAKE_CURRENT_SOURCE_DIR}/create_joint_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_joint_cmd.h"
						# build_collision
						"${CMAKE_CURRENT_SOURCE_DIR}/build_collision_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/build_collision_cmd.h"
						# nodes
						"${CMAKE_CURRENT_SOURCE_DIR}/fake_drawing_locator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/fake_drawing_locator.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/geom_sources.cpp"
						# collision
						"${CMAKE_CURRENT_SOURCE_DIR}/collision_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/collision_node.h"
						# body
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node_manip.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node_manip.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/transform_drawable.h"
						# joint
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_node.h"
						# exporter
						"${CMAKE_CURRENT_SOURCE_DIR}/node_exportable.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/node_exportable.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/tmp.cpp"
						# utils
						"${CMAKE_CURRENT_SOURCE_DIR}/export_defines.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/helpers.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/helpers.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/RenderRoutines.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/RenderRoutines.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_path_commands.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_path_commands.h"
						# 3rd_party
						# MeshMender
						"${CMAKE_CURRENT_SOURCE_DIR}/NvMender2003/NVMeshMender.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/NvMender2003/NVMeshMender.h"
						# Tootle
						"${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/amd/tootle/tootlelib.h"
						# maya_engine
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_engine.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_engine.h"
						# export
						# skinned
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_bone_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_geom_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_serialize.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skinned_surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skinned_surface.h"
						# static
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_collector.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_collector.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_writer_geom_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_writer_serialize.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/static_surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/static_surface.h"
						# base
						"${CMAKE_CURRENT_SOURCE_DIR}/model_collector.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/model_collector.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/surface.h"
						# animation
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_export_util.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_export_util.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/export_utils.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/export_utils.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_export_util.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_export_util.h"
						# old_unused
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_text_file_export.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_text_file_export.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_assemble.cpp"
						# shading
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material_hw.cpp"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_entry_point.cpp"
)

target_link_libraries(maya2018 PRIVATE
	vfs
	physics
	animation
	renderer_facade
	renderer_engine_pc_dx11
	particle
	collision
	LibFoundation
	LibPNG
#	scaleform
	bullet
	opengl32.lib
	OpenMayaUI.lib
	OpenMayaAnim.lib
	OpenMayaRender.lib
#	TootleSoftwareOnlyStatic_2k8_MTDLL64.lib
)

target_link_directories(maya2018 PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/xray/maya/maya2018_lib_x64
#	${SOLUTION_DIR_INCLUDE_PATH}/third_party/amd_tootle-master
)

set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp PROPERTY VS_SETTINGS "PrecompiledHeader=Create")

#target_precompile_headers(maya2018 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

set_target_properties(maya2018 PROPERTIES FOLDER "engine/plugins")

#########################################

add_library(maya2019 SHARED
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_library_linkage.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_memory.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/node_identifiers.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						# maya_engine
						# physics
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_body.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_body.h"
						# commands
						# create_body
						"${CMAKE_CURRENT_SOURCE_DIR}/create_body_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_body_cmd.h"
						# create_collision
						"${CMAKE_CURRENT_SOURCE_DIR}/create_collision_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_collision_cmd.h"
						# create_joint
						"${CMAKE_CURRENT_SOURCE_DIR}/create_joint_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_joint_cmd.h"
						# build_collision
						"${CMAKE_CURRENT_SOURCE_DIR}/build_collision_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/build_collision_cmd.h"
						# nodes
						"${CMAKE_CURRENT_SOURCE_DIR}/fake_drawing_locator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/fake_drawing_locator.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/geom_sources.cpp"
						# collision
						"${CMAKE_CURRENT_SOURCE_DIR}/collision_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/collision_node.h"
						# body
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node_manip.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node_manip.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/transform_drawable.h"
						# joint
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_node.h"
						# exporter
						"${CMAKE_CURRENT_SOURCE_DIR}/node_exportable.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/node_exportable.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/tmp.cpp"
						# utils
						"${CMAKE_CURRENT_SOURCE_DIR}/export_defines.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/helpers.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/helpers.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/RenderRoutines.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/RenderRoutines.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_path_commands.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_path_commands.h"
						# 3rd_party
						# MeshMender
						"${CMAKE_CURRENT_SOURCE_DIR}/NvMender2003/NVMeshMender.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/NvMender2003/NVMeshMender.h"
						# Tootle
						"${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/amd/tootle/tootlelib.h"
						# maya_engine
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_engine.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_engine.h"
						# export
						# skinned
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_bone_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_geom_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_serialize.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skinned_surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skinned_surface.h"
						# static
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_collector.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_collector.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_writer_geom_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_writer_serialize.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/static_surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/static_surface.h"
						# base
						"${CMAKE_CURRENT_SOURCE_DIR}/model_collector.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/model_collector.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/surface.h"
						# animation
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_export_util.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_export_util.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/export_utils.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/export_utils.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_export_util.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_export_util.h"
						# old_unused
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_text_file_export.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_text_file_export.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_assemble.cpp"
						# shading
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material_hw.cpp"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_entry_point.cpp"
)

target_link_libraries(maya2019 PRIVATE
	vfs
	physics
	animation
	renderer_facade
	renderer_engine_pc_dx11
	particle
	collision
	LibFoundation
	LibPNG
#	scaleform
	bullet
	opengl32.lib
	OpenMayaUI.lib
	OpenMayaAnim.lib
	OpenMayaRender.lib
#	TootleSoftwareOnlyStatic_2k8_MTDLL64.lib
)

target_link_directories(maya2019 PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/xray/maya/maya2019_lib_x64
#	${SOLUTION_DIR_INCLUDE_PATH}/third_party/amd_tootle-master
)

set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp PROPERTY VS_SETTINGS "PrecompiledHeader=Create")

#target_precompile_headers(maya2019 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

set_target_properties(maya2019 PROPERTIES FOLDER "engine/plugins")

#########################################

add_library(maya2023 SHARED
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_library_linkage.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_memory.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/node_identifiers.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						# maya_engine
						# physics
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_body.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_body.h"
						# commands
						# create_body
						"${CMAKE_CURRENT_SOURCE_DIR}/create_body_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_body_cmd.h"
						# create_collision
						"${CMAKE_CURRENT_SOURCE_DIR}/create_collision_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_collision_cmd.h"
						# create_joint
						"${CMAKE_CURRENT_SOURCE_DIR}/create_joint_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/create_joint_cmd.h"
						# build_collision
						"${CMAKE_CURRENT_SOURCE_DIR}/build_collision_cmd.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/build_collision_cmd.h"
						# nodes
						"${CMAKE_CURRENT_SOURCE_DIR}/fake_drawing_locator.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/fake_drawing_locator.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/geom_sources.cpp"
						# collision
						"${CMAKE_CURRENT_SOURCE_DIR}/collision_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/collision_node.h"
						# body
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node_manip.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_node_manip.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/transform_drawable.h"
						# joint
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_node.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_node.h"
						# exporter
						"${CMAKE_CURRENT_SOURCE_DIR}/node_exportable.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/node_exportable.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/tmp.cpp"
						# utils
						"${CMAKE_CURRENT_SOURCE_DIR}/export_defines.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/helpers.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/helpers.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/RenderRoutines.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/RenderRoutines.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_path_commands.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_path_commands.h"
						# 3rd_party
						# MeshMender
						"${CMAKE_CURRENT_SOURCE_DIR}/NvMender2003/NVMeshMender.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/NvMender2003/NVMeshMender.h"
						# Tootle
						"${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/amd/tootle/tootlelib.h"
						# maya_engine
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_engine.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_engine.h"
						# export
						# skinned
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_bone_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_geom_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skin_visual_writer_serialize.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skinned_surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skinned_surface.h"
						# static
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_collector.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_collector.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_exporter.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_exporter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_writer_geom_extractor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/solid_visual_writer_serialize.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/static_surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/static_surface.h"
						# base
						"${CMAKE_CURRENT_SOURCE_DIR}/model_collector.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/model_collector.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/surface.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/surface.h"
						# animation
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_export_util.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_export_util.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/export_utils.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/export_utils.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_export_util.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/skeleton_export_util.h"
						# old_unused
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_text_file_export.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_text_file_export.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/anim_track_assemble.cpp"
						# shading
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/xray_material_hw.cpp"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/maya_entry_point.cpp"
)

target_link_libraries(maya2023 PRIVATE
	vfs
	physics
	animation
	renderer_facade
	renderer_engine_pc_dx11
	particle
	collision
	LibFoundation
	LibPNG
#	scaleform
	bullet
	opengl32.lib
	OpenMayaUI.lib
	OpenMayaAnim.lib
	OpenMayaRender.lib
#	TootleSoftwareOnlyStatic_2k8_MTDLL64.lib
)

target_link_directories(maya2023 PRIVATE
	${SOLUTION_DIR_INCLUDE_PATH}/xray/maya/maya2023_lib_x64
#	${SOLUTION_DIR_INCLUDE_PATH}/third_party/amd_tootle-master
)

set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp PROPERTY VS_SETTINGS "PrecompiledHeader=Create")

#target_precompile_headers(maya2023 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pch.h")

set_target_properties(maya2023 PROPERTIES FOLDER "engine/plugins")

