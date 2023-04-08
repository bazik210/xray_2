////////////////////////////////////////////////////////////////////////////
//	Created		: 07.07.2011
//	Author		: Alexander Stupakov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef BSP_TREE_POLYGON_H_INCLUDED
#define BSP_TREE_POLYGON_H_INCLUDED
#include "indexed_polygon.h"
namespace xray {
namespace model_editor {
class bsp_tree_polygon : public indexed_polygon
{
public:
							bsp_tree_polygon			();//empty polygon
							bsp_tree_polygon			( coord_vector&	vertices, index_vector& indices, u32 index_offset, u32 vertices_count );
							bsp_tree_polygon			( coord_vector&	vertices, index_vector& indices, u32 index_offset, u32 vertices_count, math::plane const& p );
	bool					is_divider					( ) const			{ return m_is_divider; }
	void					set_divider					( bool divider )	{ m_is_divider = divider; }
private:
	bool					m_is_divider;
}; // class bsp_tree_polygon

bool is_polygon_set_valid	( polygon_vector const& polygons );
bool is_convex_polygon_set	( polygon_vector const& polygons );
bool is_polygon_set_coplanar( polygon_vector const& polygons );

} // namespace model_editor
} // namespace xray


#endif // #ifndef BSP_TREE_POLYGON_H_INCLUDED