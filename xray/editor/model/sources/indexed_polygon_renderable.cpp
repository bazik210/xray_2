////////////////////////////////////////////////////////////////////////////
//	Created		: 02.08.2011
//	Author		: Alexander Stupakov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "indexed_polygon.h"
#pragma managed(push, off)
#include <xray/render/facade/debug_renderer.h>
#pragma managed(pop)

namespace xray {
namespace model_editor {

void	render_indexed_polygon	( indexed_polygon const* polygon, render::scene_ptr const& scene, render::debug::renderer& r, math::color const& c, bool solid, bool double_sided )
{
	if ( polygon->get_vertices_count() < 3 )//FIXME
		return;
	R_ASSERT( polygon->get_vertices_count() >= 2);

	math::color plane_color( c.r, c.g, c.b, 128 );
	math::float3 const& first_point = polygon->vertex( 0 );
	r.draw_line( scene, first_point, polygon->vertex( 1 ), c );
	for ( u32 i = 1; i <  polygon->get_vertices_count() - 1; ++i )
	{
		math::float3 const& current_point	= polygon->vertex( i );
		math::float3 const& next_point		= polygon->vertex( i  + 1);
		r.draw_line( scene, current_point, next_point, c );
		if ( solid )
		{
			r.draw_triangle( scene, first_point, current_point, next_point, plane_color );
			if ( double_sided )
				r.draw_triangle( scene, first_point, next_point, current_point, plane_color );
		}
	}
	math::float3 const& last_point = polygon->vertex( polygon->get_vertices_count() - 1 );
	r.draw_line( scene, last_point, first_point, c );
}


} // namespace model_editor
} // namespace xray

