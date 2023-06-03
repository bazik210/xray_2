project(collision LANGUAGES CXX)

add_library(collision STATIC
                        # core
                        "${CMAKE_CURRENT_SOURCE_DIR}/collision_library_linkage.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/opcode_include.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/pch.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
                        # include
                        "${CMAKE_CURRENT_SOURCE_DIR}/../animated_object.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../api.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../bone_collision_data.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../collision_object.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../common_types.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../contact_info.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../geometry.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../geometry_double_dispatcher.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../geometry_instance.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../library_linkage.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../object.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../primitives.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../ray_collision_utils.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../ray_collision_utils_inline.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../shell_creation_utils.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../shell_creation_utils_inline.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../space_partitioning_tree.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../terrain_collision_utils.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/../terrain_collision_utils_inline.h"
                        # colliders
                        # ray
                        # base
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_ray_aabb.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_ray_aabb_all.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_ray_aabb_win.cpp"
                        # geometry
                        # base
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_ray_geometry_base.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_ray_geometry_base.h"
                        # query
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_ray_query_geometry.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_ray_query_geometry.h"
                        # test
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_ray_test_geometry.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_ray_test_geometry.h"
                        # object
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_ray_object.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_ray_object.h"
                        # aabb
                        # geometry
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_aabb_geometry.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_aabb_geometry.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_aabb_geometry_triangle_intersection.cpp"
                        # object
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_aabb_object.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_aabb_object.h"
                        # base
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_aabb_base.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_aabb_base_inline.h"
                        # cuboid
                        # geometry
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_cuboid_geometry.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_cuboid_geometry.h"
                        # object
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_cuboid_object.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_cuboid_object.h"
                        # convex
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_convex_geometry.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/colliders_convex_geometry.h"
                        # loose_oct_tree
                        "${CMAKE_CURRENT_SOURCE_DIR}/loose_oct_tree.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/loose_oct_tree.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/loose_oct_tree_inline.h"
                        # geometry
                        # object
                        "${CMAKE_CURRENT_SOURCE_DIR}/aabb_object.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/aabb_object.h"
                        # geometries
                        # triangle_mesh
                        # resource
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_resource.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_resource.h"
                        # base
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_base_inline.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_geometry.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_geometry.h"
                        # buffer
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_buffer.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_buffer.h"
                        # primitive_collision
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_primitiv_res.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_primitiv_res.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_primitiv_res_inline.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_primitive_contact_tests.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_primitive_contact_tests.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_primitive_contact_tests_inline.h"
                        # triangle_sphere
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_sphere.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_sphere.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_sphere.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_sphere_inline.h"
                        # triangle_sphere_contact_tests
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_sphere_contact_tests.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_sphere_contact_tests.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_sphere_contact_tests_inline.h"
                        # triangle_cylinder
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_cylinder_contact_tests.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_cylinder_contact_tests.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_cylinder_contact_tests_inline.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_cylinder_res.h"
                        # triangle_box
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_box_contact_tests.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_box_contact_tests_inline.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_box_contact_tests_old.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_box_res.h"
                        # composite
                        "${CMAKE_CURRENT_SOURCE_DIR}/composite_geometry.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/composite_geometry.h"
                        # cook
                        "${CMAKE_CURRENT_SOURCE_DIR}/collision_cook.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/collision_cook.h"
                        # dispatchers
                        # containment
                        "${CMAKE_CURRENT_SOURCE_DIR}/containment_double_dispatcher.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/containment_double_dispatcher.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/containment_double_dispatcher_box.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/containment_double_dispatcher_capsule.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/containment_double_dispatcher_cylinder.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/containment_double_dispatcher_sphere.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/containment_double_dispatcher_truncated_sphere.cpp"
                        # intersection
                        "${CMAKE_CURRENT_SOURCE_DIR}/intersection_double_dispatcher.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/intersection_double_dispatcher.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/intersection_double_dispatcher_box.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/intersection_double_dispatcher_capsule.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/intersection_double_dispatcher_cylinder.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/intersection_double_dispatcher_sphere.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/intersection_double_dispatcher_truncated_sphere.cpp"
                        # geometry_instaces
                        # terrain
                        "${CMAKE_CURRENT_SOURCE_DIR}/terrain_data.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/terrain_data.h"
                        "${CMAKE_CURRENT_SOURCE_DIR}/terrain_geometry_instance.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/terrain_geometry_instance.h"
                        # box
                        "${CMAKE_CURRENT_SOURCE_DIR}/box_geometry_instance.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/box_geometry_instance.h"
                        # cylinder
                        "${CMAKE_CURRENT_SOURCE_DIR}/cylinder_geometry_instance.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/cylinder_geometry_instance.h"
                        # sphere
                        "${CMAKE_CURRENT_SOURCE_DIR}/sphere_geometry_instance.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/sphere_geometry_instance.h"
                        # triangle_mesh
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_geometry_instance.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/triangle_mesh_geometry_instance.h"
                        # composite
                        "${CMAKE_CURRENT_SOURCE_DIR}/composite_geometry_instance.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/composite_geometry_instance.h"
                        # truncated_sphere
                        "${CMAKE_CURRENT_SOURCE_DIR}/truncated_sphere_geometry_instance.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/truncated_sphere_geometry_instance.h"
                        # capsule
                        "${CMAKE_CURRENT_SOURCE_DIR}/capsule_geometry_instance.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/capsule_geometry_instance.h"
                        # animated_object
                        "${CMAKE_CURRENT_SOURCE_DIR}/animated_object.cpp"
                        # ./
                        "${CMAKE_CURRENT_SOURCE_DIR}/api.cpp"
                        "${CMAKE_CURRENT_SOURCE_DIR}/collision_object.cpp"
)

target_link_libraries(collision PUBLIC
	core
	opcode
)

# File grouping for Visual Studio Solution Explorer
source_group("animated_object" FILES
                        "${CMAKE_CURRENT_SOURCE_DIR}/animated_object.cpp"
)





# For copy-pasting
#source_group("folder/path" FILES
#                       "${CMAKE_CURRENT_SOURCE_DIR}/filename.ext"
#)

# Project settings (i.e. additional include directories, macros etc.)
set_target_properties(collision PROPERTIES FOLDER "engine")