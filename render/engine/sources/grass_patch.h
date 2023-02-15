////////////////////////////////////////////////////////////////////////////
//	Created 	: 26.03.2012
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_RENDER_GRASS_PATCH_H_INCLUDED
#define XRAY_RENDER_GRASS_PATCH_H_INCLUDED

#include "grass_template.h"
#include <xray/resources_cook_classes.h>

namespace xray {

namespace collision {
	struct space_partitioning_tree;
	class geometry_instance;
	class object;
} // namespace collision

namespace render {

class untyped_buffer;
class resource_intrusive_base;
typedef	intrusive_ptr<
	untyped_buffer,
	resource_intrusive_base,
	threading::single_threading_policy
> untyped_buffer_ptr;

class res_declaration;
typedef xray::intrusive_ptr<res_declaration, resource_intrusive_base, threading::single_threading_policy>	ref_declaration;

struct grass_patch: public boost::noncopyable
{
												grass_patch				(collision::space_partitioning_tree* const in_collision_tree,
																		 grass_template* templ,
																		 math::float3 const& in_origin,
																		 math::float3 const& in_sizes);
												~grass_patch			();
	void										create_render_buffer	();
	u32											get_instance_data_stride() const;
	math::aabb									m_aabb;
	math::float3								m_origin;
	math::float3								m_sizes;
	grass_instances_type						m_instances;
	untyped_buffer_ptr							m_instance_vb;
	ref_declaration								m_instance_decl;
	grass_template*								m_template;
	collision::space_partitioning_tree* const	m_collision_tree;
	collision::geometry_instance*				m_collision_geometry;
	collision::object*							m_collision_object;
	bool										m_visible;
}; // struct grass_patch

} // namespace render
} // namespace xray

#endif // #ifndef XRAY_RENDER_GRASS_PATCH_H_INCLUDED