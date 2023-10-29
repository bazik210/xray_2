////////////////////////////////////////////////////////////////////////////
//	Created		: 01.08.2011
//	Author		: Alexander Stupakov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef INDEXED_POLYGON_H_INCLUDED
#define INDEXED_POLYGON_H_INCLUDED
#include "indexed_edge.h"
#include "bsp_typedefs.h"

namespace xray {
namespace model_editor {

class indexed_polygon;
typedef vector<indexed_polygon*> polygon_vector;

class indexed_polygon {
public:
	static float const msc_square_threshold;
	static float ms_similarity_max_square_difference;
	static float ms_similarity_max_position_difference;
	enum polygon_side { polygon_side_front, polygon_side_back, polygon_side_coincident, polygon_side_spanning };
	static polygon_side		calculate_side				( math::plane const& self, indexed_polygon const& other );
							indexed_polygon				();//empty polygon
							indexed_polygon				( coord_vector&	vertices, index_vector& indices, u32 index_offset, u32 vertices_count );
							indexed_polygon				( coord_vector&	vertices, index_vector& indices, u32 index_offset, u32 vertices_count, math::plane const& p );
	math::aabb				get_aabb					( ) const;
	bool					valid						( ) const;
	void					set_plane					( math::plane const& p ) { m_plane = p; }
	math::plane	const&		get_plane					( ) const { return m_plane; }
	u32						index						( u32 index ) const { return (*m_indices)[ m_index_offset + index ]; }
	math::float3 const&		vertex						( u32 index ) const { return (*m_vertices)[ (*m_indices)[ m_index_offset + index ] ]; }
	u32						get_vertices_count			( ) const { return m_vertices_count; }
	void					split						( math::plane const& divider, indexed_polygon& positive, indexed_polygon& negative ) const;
	void					split_in_place				( math::plane const& divider, indexed_polygon& other );
	float					get_square					()const { return m_square; }
	float					get_slimness				()const { return m_slimness; }
	bool					contains					( indexed_edge const& edge ) const;
	bool					contains					( u32 index ) const;
	void					remove_double_vertices		();
	bool					is_coplanar					( const math::plane& p ) const;
	bool					enough_vertices				() const { return m_vertices_count >= 3; }
	bool					is_convex					() const;
	void					split_to_convex				( polygon_vector& convex_polygons ) const;
protected:
	void					split_indices_impl			(  math::plane const& divider, index_vector& positive_indices, index_vector& negative_indices ) const;
	void					update_square_and_slimness	();
	void					split_in_place_nonconvex	( math::plane const& divider, indexed_polygon& other, u32 special_index );
	void					initialise_this_and_another	 ( index_vector const& positive_indices, index_vector const& negative_indices, indexed_polygon& other );

	coord_vector*			m_vertices;
	index_vector* 			m_indices;
	u32						m_index_offset;
	math::plane				m_plane;
	u32						m_vertices_count;
	u32						m_max_vertices_count;
	float					m_square;
	float					m_slimness;
}; // class indexed_polygon

float indexed_polygon_square( coord_vector const& vertices, index_vector const& indices );

void get_polygons_edges			( polygon_vector const& polygons, edges_buffer_type& edges );
void get_polygons_outer_edges	( polygon_vector const& polygons, edges_buffer_type& edges );
bool is_point_inside_polygon	( indexed_polygon const& polygon, math::float3 const& point );
} // namespace model_editor
} // namespace xray

#endif // #ifndef INDEXED_POLYGON_H_INCLUDED