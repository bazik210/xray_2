////////////////////////////////////////////////////////////////////////////
//	Created		: 14.12.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "scene_view_xbox360.h"

namespace xray {
namespace render {

scene_view::scene_view():
	m_view_mode(lit_view_mode),
	//m_particles_render_mode(xray::particle::normal_particle_render_mode),
	m_use_post_process(true),
	m_render_frame_index(0)
{
	m_prev_frame_luminance_parameters	= math::float4(0.0f, 0.0f, 0.0f, 0.0f);
	m_frame_luminance_parameters		= math::float4(0.0f, 0.0f, 0.0f, 0.0f);
}

scene_view::~scene_view()
{
	
}

} // namespace render
} // namespace xray


