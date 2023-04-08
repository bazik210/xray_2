////////////////////////////////////////////////////////////////////////////
//	Created		: 01.08.2011
//	Author		: Alexander Stupakov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "indexed_polygon.h"
#include "bsp_tree_utilities.h"

#include <xray/ai_navigation/sources/triangles_mesh.h>
#include <xray/ai_navigation/sources/delaunay_triangulator.h>
#include <xray/ai_navigation/sources/constrained_delaunay_triangulator.h>

namespace xray {
namespace model_editor {

float const indexed_polygon::msc_square_threshold = math::epsilon_5;
float indexed_polygon::ms_similarity_max_square_difference = 1e-2f;
float indexed_polygon::ms_similarity_max_position_difference = 2.5e-2f;


indexed_polygon::polygon_side indexed_polygon::calculate_side( math::plane const& p, indexed_polygon const& poly )
{
	//if ( this->is_coplanar( p ) )
	//	return polygon_side_coincident;
	//u32 positive = 0, negative = 0;
	//for ( u32 i = 0; i < poly.get_vertices_count(); ++i )
	//{
	//	float dist = p.classify( (*poly.m_vertices)[ (*poly.m_indices)[ poly.m_index_offset + i ] ] );
	//	if ( math::is_zero( dist, math::epsilon_3 ) )
	//		continue;
	//	else if ( dist > 0.0f )
	//		++positive;
	//	else
	//		++negative;
	//}
	//if ( positive > 0 && negative == 0) 
	//	return polygon_side_front; 
	//else if( positive == 0 && negative > 0 ) 
	//{
	//	return polygon_side_back;
	//}
	//else if( positive == 0 && negative == 0 )
	//{
	//	if ( poly.is_coplanar( p ) )
	//		return polygon_side_coincident;
	//	else
	//	{
	//		return ( p.normal | poly.m_plane.normal ) > 0.0f  ? polygon_side_front : polygon_side_back;
	//	}
	//}
	//else 
		return polygon_side_spanning; 
}
							
indexed_polygon::indexed_polygon():
m_vertices( NULL ),
m_indices( NULL ),
m_index_offset( 0 ),
m_vertices_count( 0 ),
m_max_vertices_count( 0 ),
m_square( math::QNaN ),
m_slimness( math::QNaN )
{
}

indexed_polygon::indexed_polygon( coord_vector&	vertices, index_vector& indices, u32 index_offset, u32 vertices_count ):
m_vertices( &vertices ),
m_indices( &indices ),
m_index_offset( index_offset ),
m_vertices_count( vertices_count ),
m_max_vertices_count( vertices_count ),
m_square( math::QNaN ),
m_slimness( math::QNaN )
{
	R_ASSERT( m_vertices_count >= 3 );
	m_plane = math::create_plane( (*m_vertices)[ (*m_indices)[m_index_offset] ], (*m_vertices)[ (*m_indices)[m_index_offset + 1] ], (*m_vertices)[ (*m_indices)[m_index_offset + 2] ] );
	//for ( u32 i = 0; i < vertices_count; ++i )
	//{
	//	R_ASSERT( math::is_zero( m_plane.classify( this->vertex( i ) ) ) );
	//}
	R_ASSERT( m_plane.valid() );
	this->update_square_and_slimness();
}

indexed_polygon::indexed_polygon( coord_vector&	vertices, index_vector& indices, u32 index_offset, u32 vertices_count, math::plane const& p ):
m_vertices( &vertices ),
m_indices( &indices ),
m_index_offset( index_offset ),
m_plane( p ),
m_vertices_count( vertices_count ),
m_max_vertices_count( vertices_count ),
m_square( math::QNaN ),
m_slimness( math::QNaN )
{
	this->update_square_and_slimness();
}

math::aabb indexed_polygon::get_aabb( ) const
{
	R_ASSERT( m_vertices_count >= 3 );
	math::aabb bbox = create_aabb_min_max( (*m_vertices)[ (*m_indices)[m_index_offset] ], (*m_vertices)[ (*m_indices)[m_index_offset + 1] ] );
	for ( u32 i = 2; i < m_vertices_count; ++i )
	{
		bbox.modify( (*m_vertices)[ (*m_indices)[m_index_offset + i] ] );
	}
	return bbox;
}

bool indexed_polygon::valid( ) const
{
	return true;//TODO
}

void indexed_polygon::split( math::plane const& divider, indexed_polygon& positive, indexed_polygon& negative ) const
{
	R_ASSERT( m_plane.valid() );
	if ( !m_vertices_count)
		return;
	index_vector positive_indices, negative_indices;
	split_indices_impl( divider, positive_indices, negative_indices );
	if ( !positive_indices.empty() )
	{
		positive.m_index_offset = (*m_indices).size();
		(*m_indices).insert( (*m_indices).end(), positive_indices.begin(), positive_indices.end() );
		positive.m_vertices_count = positive_indices.size();
		positive.m_max_vertices_count = positive_indices.size();
		positive.m_plane = m_plane;
		positive.m_indices = m_indices;
		positive.m_vertices = m_vertices;
		positive.update_square_and_slimness();
	}
	if ( !negative_indices.empty() )
	{
		negative.m_index_offset = (*m_indices).size();
		(*m_indices).insert( (*m_indices).end(), negative_indices.begin(), negative_indices.end() );
		negative.m_vertices_count = negative_indices.size();
		negative.m_max_vertices_count = negative_indices.size();
		negative.m_plane = m_plane;
		negative.m_indices = m_indices;
		negative.m_vertices = m_vertices;
		negative.update_square_and_slimness();
	}
}

void indexed_polygon::split_in_place( math::plane const& divider, indexed_polygon& other )
{
	R_ASSERT( m_plane.valid() );
	if ( !m_vertices_count)
		return;
	index_vector positive_indices, negative_indices;
	split_indices_impl( divider, positive_indices, negative_indices );
	this->initialise_this_and_another( positive_indices, negative_indices, other );
}

void indexed_polygon::initialise_this_and_another ( index_vector const& positive_indices, index_vector const& negative_indices, indexed_polygon& other )
{
	//R_ASSERT( !positive_indices.empty() || !negative_indices.empty() );
	if ( !positive_indices.empty() )
	{
		if ( m_max_vertices_count >= positive_indices.size() )
		{
			m_vertices_count = positive_indices.size();
			std::copy( positive_indices.begin(), positive_indices.end(), (*m_indices).begin() + m_index_offset );
		}
		else
		{
			m_index_offset = (*m_indices).size();
			(*m_indices).insert( (*m_indices).end(), positive_indices.begin(), positive_indices.end() );
			m_vertices_count = positive_indices.size();
			m_max_vertices_count = positive_indices.size();
		}
		this->update_square_and_slimness();
	}
	if ( !negative_indices.empty() )
	{
		other.m_index_offset = (*m_indices).size();
		(*m_indices).insert( (*m_indices).end(), negative_indices.begin(), negative_indices.end() );
		other.m_vertices_count = negative_indices.size();
		other.m_max_vertices_count = negative_indices.size();
		other.m_plane = m_plane;
		other.m_indices = m_indices;
		other.m_vertices = m_vertices;
		other.update_square_and_slimness();
	}
}

void indexed_polygon::split_indices_impl(  math::plane const& divider, index_vector& positive_indices, index_vector& negative_indices ) const
{
	R_ASSERT( m_plane.valid() );
	math::float3 intersection;
	for ( u32 i = 0; i < m_vertices_count; ++i )
	{
		u32 const current					= this->index( i );
		u32 const next						= this->index( ( i + 1 ) % m_vertices_count );
		math::float3 const& current_vertex	= ( *m_vertices )[ current ];
		math::float3 const& next_vertex		= ( *m_vertices )[ next ];

		float dist		= divider.classify( current_vertex );
		float next_dist = divider.classify( next_vertex );
		if ( ( dist > 0.0f ) || ( math::is_zero( dist ) && next_dist >= 0 ) )
			positive_indices.push_back( current );
		else
			negative_indices.push_back( current );

		if ( divider.intersect_segment( current_vertex, next_vertex, intersection ) )
		{
			R_ASSERT( intersection.valid() );
			if ( !math::is_zero( m_plane.classify( intersection ) ) )
				intersection = m_plane.project( intersection );
			if ( !is_between( intersection, current_vertex, next_vertex ) )
				make_point_between( current_vertex, next_vertex, intersection );
			u32 const intersection_index = m_vertices->size();
			m_vertices->push_back( intersection );
			positive_indices.push_back( intersection_index );
			negative_indices.push_back( intersection_index );
		}
	}
	//bool const ok = 
	//	positive_indices.empty() && ( m_vertices_count == negative_indices.size() ) ||
	//	negative_indices.empty() && ( m_vertices_count == positive_indices.size() ) ||
	//	!positive_indices.empty() && !negative_indices.empty() && ( positive_indices.size() + negative_indices.size() == m_vertices_count + 4 );
	//R_ASSERT( ok );
}

void indexed_polygon::update_square_and_slimness() 
{
	m_square = 0.0f;
	if ( m_vertices_count < 3 )
		return;
	math::float3 const& p0 = this->vertex( 0 );
	for ( u32 i = 1; i < m_vertices_count - 1; ++i )
	{
		math::float3 const& p1 = this->vertex( i );
		math::float3 const& p2 = this->vertex( i + 1 );
		m_square += ( ( p1 - p0 ) ^ ( p2 - p0 ) ).length() * 0.5f;
	}
	float perimeter = 0.0f;
	for ( u32 i = 0; i < m_vertices_count; ++i )
	{
		u32 const next = ( i + 1 ) % m_vertices_count;
		perimeter += ( this->vertex( i ) - this->vertex( next ) ).length();
	}
	m_slimness = m_square / perimeter;
}

bool indexed_polygon::contains( indexed_edge const& edge ) const
{
	for ( u32 i = 0; i < m_vertices_count; ++i )
	{
		u32 const current	= this->index( i );
		u32 const next		= this->index( ( i + 1 ) % m_vertices_count );
		if ( current == edge.i1 && next == edge.i2 || current == edge.i2 && next == edge.i1 )
			return true;
	}
	return false;
}

bool indexed_polygon::contains( u32 index ) const
{
	index_vector::const_iterator const start_it = m_indices->begin() + m_index_offset;
	index_vector::const_iterator const end_it	= start_it + m_vertices_count;
	index_vector::const_iterator const it		= std::find( start_it, end_it, index );
	return it != end_it;
}

void indexed_polygon::remove_double_vertices()
{
	index_vector::iterator const first_it = m_indices->begin() + m_index_offset;
	index_vector::const_iterator const it = std::unique( first_it ,first_it + m_vertices_count );
	m_vertices_count = it - first_it;
	while ( m_vertices_count > 1 && this->index( 0 ) == this->index( m_vertices_count - 1 ) )
		m_vertices_count -= 1;
}

bool indexed_polygon::is_coplanar( const math::plane& p ) const
{
	static float const precision = 2e-2f;
	const float dot_product = p.normal | m_plane.normal;
	return math::is_similar( dot_product, 1.0f, precision) && math::is_similar( p.d, m_plane.d, precision );
}

bool indexed_polygon::is_convex() const
{
	if ( m_vertices_count <= 3 )
		return true;
	math::float3 const&	last	= this->vertex( this->get_vertices_count() - 1 );
	math::float3 const& first	= this->vertex( 0 );
	math::float3 const& second	= this->vertex( 1 );

	math::float3 const& last_to_first		= first		- last;
	math::float3 const& first_to_second		= second	- first;
	math::float3 const& cross				= last_to_first ^ first_to_second;
	for ( u32 i = 0; i < this->get_vertices_count() - 2; ++i )
	{
		u32 const previous	= i;
		u32 const current	= previous	+ 1;
		u32 const next		= current	+ 1;
		math::float3 const& previous_to_current = this->vertex( current )	- this->vertex( previous );
		math::float3 const& current_to_next		= this->vertex( next )		- this->vertex( current );
		float const dot = cross | ( previous_to_current ^ current_to_next );
		if ( dot < 0 )
			return false;
	}
	return true;
}

void indexed_polygon::split_to_convex ( polygon_vector& convex_polygons ) const
{
	R_ASSERT( !this->is_convex() );
	R_ASSERT( convex_polygons.empty() );
	convex_polygons.push_back( NEW( indexed_polygon )( *this ) );
	math::plane p;
	for ( u32 i = 0; i < get_vertices_count(); ++i )
	{
		u32 const next = ( i + 1 ) % get_vertices_count();
		p.normal	= m_plane.normal ^ ( this->vertex( next ) - this->vertex( i ) );
		p.d			= -p.normal | this->vertex( i );
		for ( u32 j = 0; j < convex_polygons.size(); ++j )
		{
			indexed_polygon* current = convex_polygons[ j ];
			if ( current->is_convex() || indexed_polygon::calculate_side( p, *current ) != indexed_polygon::polygon_side_spanning )
				continue;
			indexed_polygon other;
			current->split_in_place_nonconvex( p, other, i );
			if ( current->get_square() < indexed_polygon::msc_square_threshold && other.enough_vertices() )
			{
				*current = other;
			} 
			else if ( other.get_square() >= indexed_polygon::msc_square_threshold )
				convex_polygons.push_back( NEW(indexed_polygon)( other ) );
		}
	}
	convex_polygons.erase( std::remove_if( convex_polygons.begin(), convex_polygons.end(), std::not1( std::mem_fun( &indexed_polygon::enough_vertices ) ) ), convex_polygons.end() );
	//for ( polygon_vector::const_iterator it = convex_polygons.begin(); it != convex_polygons.end(); ++it )
	//{
	//	R_ASSERT( (*it)->is_convex() );
	//}

}

void indexed_polygon::split_in_place_nonconvex( math::plane const& divider, indexed_polygon& other, u32 special_index )
{
	R_ASSERT( !this->is_convex() );
	index_vector positive_indices, negative_indices;
	math::float3 intersection;
	for ( u32 i = 0; i < m_vertices_count; ++i )
	{
		u32 const current					= this->index( i );
		u32 const next						= this->index( ( i + 1 ) % m_vertices_count );
		math::float3 const& current_vertex	= ( *m_vertices )[ current ];
		math::float3 const& next_vertex		= ( *m_vertices )[ next ];

		float const dist		= divider.classify( current_vertex );
		float const next_dist = divider.classify( next_vertex );
		if ( i == special_index )
		{
			math::float3 const& previous_vertex		= this->vertex( ( i - 1 + m_vertices_count ) % m_vertices_count );
			float const previous_dist				= divider.classify( previous_vertex );
			if ( previous_dist >= 0 )
				positive_indices.push_back( current );
			else
				negative_indices.push_back( current );
			
			u32 const after_next  = ( i + 2 ) % m_vertices_count;
			++i;
			if ( after_next == 0 )
				continue;
				math::float3 const& after_next_vertex	= this->vertex( after_next );
				float const after_next_dist				= divider.classify( after_next_vertex );
				if ( after_next_dist >= 0 )
					positive_indices.push_back( next );
				else
					negative_indices.push_back( next );

			continue;
		}
		else if ( ( dist > 0.0f ) || ( math::is_zero( dist ) && next_dist >= 0 ) )
			positive_indices.push_back( current );
		else
			negative_indices.push_back( current );

		if ( math::sign( dist ) != math::sign( next_dist ) && divider.intersect_segment( current_vertex, next_vertex, intersection ) )
		{
			R_ASSERT( intersection.valid() );
			if ( !math::is_zero( m_plane.classify( intersection ) ) )
				intersection = m_plane.project( intersection );
			if ( !is_between( intersection, current_vertex, next_vertex ) )
				make_point_between( current_vertex, next_vertex, intersection );
			u32 const intersection_index = m_vertices->size();
			m_vertices->push_back( intersection );
			positive_indices.push_back( intersection_index );
			negative_indices.push_back( intersection_index );
		}
	}
	//remove_unnecessary_vertices_from_circuit( *m_vertices, positive_indices );
	//remove_unnecessary_vertices_from_circuit( *m_vertices, negative_indices );
	this->initialise_this_and_another( positive_indices, negative_indices, other );
}

float indexed_polygon_square( coord_vector const& vertices, index_vector const& indices )
{
	float result = 0.0f;
	for ( u32 i = 0; i < indices.size() - 2; ++i )
	{
		math::float3 const& p0 = vertices[ indices[ i ] ];
		math::float3 const& p1 = vertices[ indices[ ( i + 1 ) % indices.size() ] ];
		math::float3 const& p2 = vertices[ indices[ ( i + 2 ) % indices.size() ] ];
		result += ( ( p1 - p0 ) ^ ( p2 - p0 ) ).length() * 0.5f;
	}
	return result;
}

void get_polygons_edges( polygon_vector const& polygons, edges_buffer_type& edges )
{
	const polygon_vector::const_iterator end_it = polygons.end();
	for ( polygon_vector::const_iterator it = polygons.begin(); it != end_it; ++it )
	{
		indexed_polygon const& current = **it;
		for ( u32 i = 0; i < current.get_vertices_count(); ++i )
		{
			u32 next_point = ( i + 1 ) % current.get_vertices_count();
			indexed_edge edge( current.index( i ), current.index( next_point ) );
			edges.push_back( edge );
		}
	}
	std::sort( edges.begin(), edges.end() );
	edges.erase( std::unique( edges.begin(), edges.end() ), edges.end() );
}

void get_polygons_outer_edges( polygon_vector const& polygons, edges_buffer_type& edges )
{
	typedef associative_vector<indexed_edge, u32, vector> edge_count_map;
	edge_count_map edge_count;
	const polygon_vector::const_iterator end_it = polygons.end();
	for ( polygon_vector::const_iterator it = polygons.begin(); it != end_it; ++it )
	{
		indexed_polygon const& current = **it;
		for ( u32 i = 0; i < current.get_vertices_count(); ++i )
		{
			u32 next_point = ( i + 1 ) % current.get_vertices_count();
			indexed_edge edge( current.index ( i ), current.index( next_point ) );
			std::pair<edge_count_map::iterator, bool> res = edge_count.insert( std::make_pair( edge, 1 ) );
			if ( !res.second )
				++( res.first->second );
		}
	}
	const edge_count_map::const_iterator count_end_it = edge_count.end();
	for ( edge_count_map::const_iterator count_it = edge_count.begin(); count_it != count_end_it; ++count_it )
	{
		if ( count_it->second == 1 )
			edges.push_back( count_it->first );
	}
}

bool is_point_inside_polygon( indexed_polygon const& polygon, math::float3 const& point )
{
	static float const precision = math::epsilon_3;
	if ( polygon.get_vertices_count() < 3 )
		return false;
	if ( math::is_similar( polygon.vertex( 0 ), point, precision ) || 
		 math::is_similar( polygon.vertex( 1 ), point, precision ) ||
		 is_between( point, polygon.vertex( 0 ), polygon.vertex( 1 ) ) )
		 return true;
	math::float3 const& first_to_second		= polygon.vertex( 1 ) - polygon.vertex( 0 );
	math::float3 const& first_to_point		= point - polygon.vertex( 0 );
	math::float3 const& cross				= first_to_second ^ first_to_point;
	for ( u32 i = 1; i < polygon.get_vertices_count() - 2; ++i )
	{
		u32 const next = ( i + 1 ) % polygon.get_vertices_count();
		if ( math::is_similar( polygon.vertex( next ), point, precision ) ||
			 is_between( point, polygon.vertex( i ), polygon.vertex( next ) ) )
			 return true;

		math::float3 const& current_to_next		= polygon.vertex( next )	- polygon.vertex( i );
		math::float3 const& current_to_point	= point - polygon.vertex( i );
		float const dot = cross | ( current_to_next ^ current_to_point );
		if ( dot < 0 )
			return false;
	}
	return true;

}
} // namespace model_editor
} // namespace xray
