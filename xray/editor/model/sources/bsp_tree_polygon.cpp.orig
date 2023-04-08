////////////////////////////////////////////////////////////////////////////
//	Created		: 07.07.2011
//	Author		: Alexander Stupakov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "bsp_tree_polygon.h"
#pragma managed(push, off)
#include <xray/render/facade/debug_renderer.h>
#pragma managed(pop)

namespace xray {
namespace model_editor {


static inline float triangle_square( math::float3 const& point0, math::float3 const& point1, math::float3 const& point2 )
{
	return ( (point1 - point0) ^ ( point1 - point2 ) ).length() * 0.5f;
}

static inline float triangle_square( coord_vector const& vertices, u32 i0, u32 i1, u32 i2 )
{
	return ( (vertices[i1] - vertices[i0]) ^ ( vertices[i1] - vertices[i2] ) ).length() * 0.5f;
}

bsp_tree_polygon::bsp_tree_polygon():
m_is_divider( false )
{

}

bsp_tree_polygon::bsp_tree_polygon( coord_vector&	vertices, index_vector& indices, u32 index_offset, u32 vertices_count ):
indexed_polygon(vertices, indices, index_offset, vertices_count ),
m_is_divider( false )
{
}

bsp_tree_polygon::bsp_tree_polygon( coord_vector&	vertices, index_vector& indices, u32 index_offset, u32 vertices_count, math::plane const& p  ):
indexed_polygon(vertices, indices, index_offset, vertices_count, p ),
m_is_divider( false )
{
}

bool is_polygon_set_valid ( polygon_vector const& polygons )
{
	XRAY_UNREFERENCED_PARAMETER( polygons );
	//const bsp_polygons_type::const_iterator end_it = polygons.end();
	//for ( bsp_polygons_type::const_iterator it = polygons.begin(); it != end_it; ++it )
	//{
	//	if ( !( *it )->is_divider() )
	//		continue;	
	//	for ( bsp_polygons_type::const_iterator other_it = polygons.begin(); other_it != end_it; ++other_it )
	//	{
	//		if ( *it == *other_it )
	//		continue;
	//		bsp_tree_polygon::polygon_side side = indexed_polygon::calculate_side( (*it )->get_plane(), **other_it );
	//		if ( side == bsp_tree_polygon::polygon_side_back || side == bsp_tree_polygon::polygon_side_spanning )
	//			return false;
	//	}
	//}
	return true;
}

bool is_convex_polygon_set( polygon_vector const& polygons )
{
	const polygon_vector::const_iterator end_it = polygons.end();
	for ( polygon_vector::const_iterator it = polygons.begin(); it != end_it; ++it )
	{
		for ( polygon_vector::const_iterator other_it = polygons.begin(); other_it != end_it; ++other_it )
		{
			if ( *it == *other_it )
				continue;
			indexed_polygon::polygon_side side = indexed_polygon::calculate_side( ( *it )->get_plane(), **other_it );
			if ( side == indexed_polygon::polygon_side_back || side == indexed_polygon::polygon_side_spanning )
				return false;
		}
	}
	return true;
}

bool is_polygon_set_coplanar( polygon_vector const& polygons )
{
	const polygon_vector::const_iterator end_it = polygons.end();
	for ( polygon_vector::const_iterator it = polygons.begin(); it != end_it; ++it )
	{
		for ( polygon_vector::const_iterator other_it = polygons.begin(); other_it != end_it; ++other_it )
		{
			math::plane const& p1 = ( *it )->get_plane();
			math::plane const& p2 = ( *other_it )->get_plane();
			if ( !math::is_similar( p1.d, p2.d ) && math::is_similar( p1.normal, p2.normal ) )
				return false;
		}
	}
	return true;
}

//void select_coplanar_polygons( math::plane const& p, bsp_polygons_type const& original, bsp_polygons_type& coplanar )
//{
//	static float const precision = math::epsilon_3;
//	coplanar.reserve( original.size() );
//	const bsp_polygons_type::const_iterator end_it = original.end();
//	for ( bsp_polygons_type::const_iterator it = original.begin(); it != end_it; ++it )
//	{
//		bsp_tree_polygon* current = *it;
//		if ( math::is_similar( current->get_plane().d, p.d, precision ) && 
//			math::is_similar( current->get_plane().normal, p.normal, precision ) )
//		{
//			coplanar.push_back( current );
//		}
//	}
//}

} // namespace model_editor
} // namespace xray