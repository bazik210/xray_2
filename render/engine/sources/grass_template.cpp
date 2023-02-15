////////////////////////////////////////////////////////////////////////////
//	Created 	: 21.03.2012
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "grass_template.h"
#include "grass_instance.h"
#include "render_model_grass.h"
#include <xray/render/core/res_geometry.h>
#include <xray/render/core/res_effect.h>
#include <xray/render/core/custom_config.h>

namespace xray {
namespace render {

grass_template::grass_template(u32 const in_index, grass_render_model_ptr const& in_model_ptr):
	m_index(in_index),
	m_render_model(in_model_ptr)
{
	
}

grass_template::~grass_template()
{
	grass_instances_type::iterator	it_i		=	m_instances.begin();
	grass_instances_type::iterator	end_i		=	m_instances.end();
	
	for (; it_i != end_i; ++it_i)
	{
		grass_instance* inst					=	(*it_i);
		DELETE									(inst);
	}
}
void grass_template::apply()
{
	
}

} // namespace render
} // namespace xray
