////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.03.2012
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_RENDER_GRASS_TEMPLATE_H_INCLUDED
#define XRAY_RENDER_GRASS_TEMPLATE_H_INCLUDED

namespace xray {
namespace render {

struct grass_instance;
class grass_render_model;

typedef	resources::resource_ptr<
		grass_render_model,
		resources::unmanaged_intrusive_base
	> grass_render_model_ptr;

typedef render::vector<grass_instance*> grass_instances_type;

struct grass_template
{
			grass_template			(u32 const in_index, grass_render_model_ptr const& in_model_ptr);
			~grass_template			();
	void	apply					();
	
private:
	friend struct grass_world;
			
			grass_render_model_ptr	m_render_model;
			u32						m_index;
			grass_instances_type	m_instances;
}; // struct grass_template

} // namespace render
} // namespace xray

#endif // #ifndef XRAY_RENDER_GRASS_TEMPLATE_H_INCLUDED