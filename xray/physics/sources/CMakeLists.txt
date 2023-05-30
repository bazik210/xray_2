project(physics LANGUAGES CXX)

add_library(physics STATIC
						# core
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_library_linkage.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_memory.h"
						# include
						"${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../engine.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../model.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../shell.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../world.h"
						# world
						"${CMAKE_CURRENT_SOURCE_DIR}/generic_joint_create_lua_config.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/generic_joint_create_lua_config.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/generic_joint_create_lua_config_inline.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_world.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_world.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/shell_create_binary_config.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/shell_create_lua_config.cpp"
						# temp_storage
						"${CMAKE_CURRENT_SOURCE_DIR}/contact_storage.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/contact_storage.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/temp_storage_holder.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/temp_storage_holder.h"
						# integrator
						"${CMAKE_CURRENT_SOURCE_DIR}/integratable.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/integrator_ode.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/integrator_ode.h"
						# space
						"${CMAKE_CURRENT_SOURCE_DIR}/space_item.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/space_item.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/space_object.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/space_object.h"
						# element
						"${CMAKE_CURRENT_SOURCE_DIR}/collide_element.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/collide_element.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/collide_element_interface.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/element.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/element.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/shell_element.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/static_element.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/static_element.h"
						# body
						"${CMAKE_CURRENT_SOURCE_DIR}/auto_sleep.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/auto_sleep.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/autosleep_params.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/autosleep_params.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/body.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/body.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_ode.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/body_ode.h"
						# shell
						"${CMAKE_CURRENT_SOURCE_DIR}/shell.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/shell.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/shell_static.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/shell_static.h"
						# step_object
						"${CMAKE_CURRENT_SOURCE_DIR}/step_object.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/step_object.h"
						# scheduler
						"${CMAKE_CURRENT_SOURCE_DIR}/scheduler.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/scheduler.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/step_counter.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/step_globals.h"
						# island
						"${CMAKE_CURRENT_SOURCE_DIR}/island.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/island.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/island_object.h"
						# joint
						"${CMAKE_CURRENT_SOURCE_DIR}/joint.h"
						# contact
						"${CMAKE_CURRENT_SOURCE_DIR}/contact.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/contact.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/contact_joint_info.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/contact_ode.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/contact_ode.h"
						# contact_info
						"${CMAKE_CURRENT_SOURCE_DIR}/contact_info_buffer.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/contact_info_buffer.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/generate_contact.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/generate_contact.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/generate_contact_base.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/generate_contact_base.h"
						# joint_ode
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_ode.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_ode.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_ode_base.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_ode_base.h"
						# joint_base_impl
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_base_impl.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_base_impl.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_double_base_impl.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_double_base_impl.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_single_base_impl.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_single_base_impl.h"
						# ball
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_ball.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_ball.h"
						# euler_hinge
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_euler_hinge.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_euler_hinge.h"
						# hinge
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_hinge.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_hinge.h"
						# joint_wheel
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_wheel.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_wheel.h"
						# joint_slider_hinge
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_slider_hinge.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_slider_hinge.h"
						# fixed
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_fixed.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/joint_fixed.h"
						# ode_include
						"${CMAKE_CURRENT_SOURCE_DIR}/ode_convert.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/ode_include.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/ode_include_joint.h"
						# constants
						"${CMAKE_CURRENT_SOURCE_DIR}/integration_constants.h"
						# math_utils
						"${CMAKE_CURRENT_SOURCE_DIR}/math_utils.h"
						# static_object
						# dynamic_object
						"${CMAKE_CURRENT_SOURCE_DIR}/dynamic_object.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/dynamic_object.h"
						# pose_anchor
						"${CMAKE_CURRENT_SOURCE_DIR}/pose_anchor.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/pose_anchor.h"
						# animated_model_instance
						# cook
						"${CMAKE_CURRENT_SOURCE_DIR}/animated_model_instance_cook.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/animated_model_instance_cook.h"
						# bullet_engine_impl
						"${CMAKE_CURRENT_SOURCE_DIR}/bullet_include.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/../bullet_utils.h"
						# collision_shapes
						"${CMAKE_CURRENT_SOURCE_DIR}/collision_shapes.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/../collision_shapes.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/heightfield_terrain_shape.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/heightfield_terrain_shape.h"
						# cook
						"${CMAKE_CURRENT_SOURCE_DIR}/collision_shape_cook.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/collision_shape_cook.h"
						# world
						"${CMAKE_CURRENT_SOURCE_DIR}/bullet_physics_world.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/bullet_physics_world.h"
						# body
						"${CMAKE_CURRENT_SOURCE_DIR}/rigid_body.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/../rigid_body.h"
						"${CMAKE_CURRENT_SOURCE_DIR}/rigid_body_construction_info.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/soft_body.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/../soft_body.h"
						# joint
						# character_controller
						"${CMAKE_CURRENT_SOURCE_DIR}/character_controller.cpp"
						"${CMAKE_CURRENT_SOURCE_DIR}/../character_controller.h"
						# ray
						"${CMAKE_CURRENT_SOURCE_DIR}/../ray_result.h"
						# ./
						"${CMAKE_CURRENT_SOURCE_DIR}/physics_entry_point.cpp"
)

target_include_directories(physics PRIVATE
    ${THIRDPARTY_DIR_INCLUDE_PATH}/bullet
)

set_target_properties(physics PROPERTIES FOLDER "engine")
