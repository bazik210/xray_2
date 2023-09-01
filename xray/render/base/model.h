////////////////////////////////////////////////////////////////////////////
//	Created		: 10.03.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_RENDER_BASE_VISUAL_H_INCLUDED
#define XRAY_RENDER_BASE_VISUAL_H_INCLUDED

#include <xray/collision/geometry.h>
#include <xray/animation/i_skeleton.h>

namespace xray {

namespace collision {
	class animated_object;
} // namespace collision

namespace render {

struct model_subset_info
{
	xray::fixed_string<64>	material_instance_name;
	xray::fixed_string<64>	shading_group_name;
	u32						num_vertices;
	u32						num_indices;
	u32						primitive_count;
};

class render_model_instance_base : public resources::unmanaged_resource 
{

protected:
	XRAY_DECLARE_PURE_VIRTUAL_DESTRUCTOR( render_model_instance_base )
}; // class render_model_instance_base

typedef	resources::resource_ptr<
	render_model_instance_base,
	resources::unmanaged_intrusive_base
> render_model_instance_base_ptr;

class static_model_instance : public resources::unmanaged_resource 
{
public:
	render::render_model_instance_base_ptr	m_render_model;
	collision::geometry_ptr					m_collision_geometry;
};

typedef	resources::resource_ptr<
			static_model_instance,
			resources::unmanaged_intrusive_base
		> static_model_ptr;

class skeleton_model_instance : public resources::unmanaged_resource
{
public:
	render_model_instance_base_ptr			m_render_model;
	animation::skeleton_ptr					m_skeleton;
}; // class skeleton_model_instance

typedef	resources::resource_ptr<
			skeleton_model_instance,
			resources::unmanaged_intrusive_base >
		skeleton_model_ptr;

class terrain_model_instance : public resources::unmanaged_resource 
{
public:
	render::render_model_instance_base_ptr	m_render_model;
	collision::geometry_ptr					m_collision_geometry;
};

typedef	resources::resource_ptr<
			terrain_model_instance,
			resources::unmanaged_intrusive_base
		> terrain_model_ptr;

class skin : public resources::unmanaged_resource 
{
protected:
	XRAY_DECLARE_PURE_VIRTUAL_DESTRUCTOR( skin )
}; // class skin

typedef	resources::resource_ptr<
			skin,
			resources::unmanaged_intrusive_base
		> skin_ptr;

struct animated_model_instance : public resources::unmanaged_resource
{
	render_model_instance_base_ptr		m_model;
	skin_ptr							m_skin;
	collision::animated_object*			m_bounding_collision;
}; // struct animated_model_instance

typedef	resources::resource_ptr<
			animated_model_instance,
			resources::unmanaged_intrusive_base >
		animated_model_instance_ptr;

struct solid_model_instance
{
	render_model_instance_base_ptr		m_model;
	collision::object*					m_bounding_collision;
}; // struct solid_model_instance

} // namespace render
} // namespace xray

#endif // #ifndef XRAY_RENDER_BASE_VISUAL_H_INCLUDED