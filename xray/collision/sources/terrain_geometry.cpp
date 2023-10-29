////////////////////////////////////////////////////////////////////////////
//	Created		: 01.09.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "terrain_geometry.h"
#include <xray/collision/terrain_collision_utils.h>

namespace xray {
namespace collision {

terrain_data::terrain_data	( float phisical_size, u32 vertex_row_size, const float* heightfield ):
m_physical_size( phisical_size ),
m_vertex_row_size( vertex_row_size ),
m_heightfield( heightfield )
{

}


inline		float	terrain_data::quad_size			( )const	
{ 
	 return m_physical_size / ( quad_dimension		( ) ); 
}


inline	 	u32		terrain_data::quad_dimension		( )const	
{ 
	 return vert_row_size	( ) - 1; 
}

inline	 	float	terrain_data::min_height	( )const	
{ 
	const u32 heights_count =  vert_row_size( ) * vert_row_size( );
	
	float ret = math::infinity;
	for( u32 i = 0; i < heights_count; ++i )
		ret = math::min( ret, m_heightfield[i] );

	return ret; 
}
inline	 	float	terrain_data::max_height	( )const	
{ 
	const u32 heights_count =   vert_row_size( ) * vert_row_size( );

	float ret = -math::infinity;
	for( u32 i = 0; i < heights_count; ++i )
		ret = math::max( ret, m_heightfield[i] );

	return ret; 
}




inline	 	bool	terrain_data::get_row_col		( float3 const& position_local, int& x, int& z )const
{ 
	
	return collision::get_row_col( quad_size(), m_vertex_row_size, position_local, x, z );
}


//inline bool ray_test_quad( terrain_quad const &q, float* heights,
//				   float3 const& pos, float3 const& dir, float const max_distance, float& range )
//{
//	
//}

inline	 	float3	terrain_data::position			(u16 vertex_id) const
{
	ASSERT( vertex_id <  vert_row_size( ) * vert_row_size( ) );

	float3 result;

	terrain_vertex_xz( vert_row_size(), quad_size(), vertex_id, result.x, result.z );
	
	result.y		= m_heightfield[vertex_id];
	return result;
}

bool	terrain_data::ray_test_quad	(	int const y, int const x, 
										float3 const& ray_point, 
										float3 const& ray_dir,
										float const max_distance,
										float& range,
										bool log_out ) const
{ 
	XRAY_UNREFERENCED_PARAMETERS( y, x, &ray_point, &ray_dir, max_distance, range, log_out );
	
	if(x<0 || y<0)
		return false;

	if(x >= (int)quad_dimension() || y >= (int)quad_dimension() )
		return false;

	if(log_out)
		LOG_INFO("%d %d", x, y);

	terrain_quad	quad;
	get_quad( quad, quad_dimension(), x, y );

	return collision::ray_test_quad(	position( quad.index_lt ),  position( quad.index_rt ),
								position( quad.index_lb ),  position( quad.index_rb ),
								ray_point, ray_dir, max_distance, range );
	//m_terrain_node->get_quad	( quad, x, y );
	//return quad.ray_test		( ray_point, ray_dir, max_distance, range );

}


terrain_geometry::terrain_geometry	( float phisical_size, u32 vertex_row_size, const float* heightfield ) :
	m_data							( phisical_size, vertex_row_size, heightfield )
{
}

terrain_geometry::~terrain_geometry	( )
{
}

void terrain_geometry::destroy		( memory::base_allocator* allocator )
{
	XRAY_UNREFERENCED_PARAMETER		( allocator );
}

math::aabb&	terrain_geometry::get_aabb( math::aabb& result ) const
{
	return collision::terrain_aabb( m_data, result );
}

float3 terrain_geometry::get_random_surface_point ( math::random32& randomizer ) const
{
	XRAY_UNREFERENCED_PARAMETER	( randomizer );
	UNREACHABLE_CODE			( return float3() );
}

float terrain_geometry::get_surface_area( ) const
{
	NOT_IMPLEMENTED				( return 0.f );
}

void terrain_geometry::accept	( geometry_double_dispatcher& , geometry const& ) const
{
	NOT_IMPLEMENTED( );
}

void terrain_geometry::visit	( geometry_double_dispatcher& , box_geometry const& ) const
{
	NOT_IMPLEMENTED( );
}

void terrain_geometry::visit	( geometry_double_dispatcher& , sphere_geometry const& ) const
{
	NOT_IMPLEMENTED( );
}

void terrain_geometry::visit	( geometry_double_dispatcher& , cylinder_geometry const& ) const
{
	NOT_IMPLEMENTED( );
}

void terrain_geometry::render( render::scene_ptr const& , render::debug::renderer& , float4x4 const& ) const 
{ NOT_IMPLEMENTED( ); }
	
math::float3 const*  terrain_geometry::vertices( ) const											
{ NOT_IMPLEMENTED( return NULL ); }

u32 terrain_geometry::vertex_count( ) const
{
	NOT_IMPLEMENTED(return 0);
}

u32	terrain_geometry::index_count	( ) const
{
	NOT_IMPLEMENTED(return 0);
}

u32 const* terrain_geometry::indices( u32 ) const							
{ NOT_IMPLEMENTED( return NULL ); }

u32 const* terrain_geometry::indices( ) const							
{ NOT_IMPLEMENTED( return NULL ); }

bool terrain_geometry::aabb_query( collision::object const*, math::aabb const&, collision::triangles_type& ) const
{ NOT_IMPLEMENTED( return false ); }

bool terrain_geometry::cuboid_query( collision::object const*, math::cuboid const&, collision::triangles_type&) const 
{ NOT_IMPLEMENTED( return false ); }

bool terrain_geometry::ray_query(	collision::object const*,
											float3 const&,
											float3 const&,
											float,
											float&,
											collision::ray_triangles_type&,
											collision::triangles_predicate_type const&) const
{ NOT_IMPLEMENTED( return false ); }

bool terrain_geometry::aabb_test( math::aabb const& ) const
{ NOT_IMPLEMENTED( return false ); }

bool terrain_geometry::cuboid_test( math::cuboid const& ) const	
{ return false;}

bool terrain_geometry::ray_test( math::float3 const& p_os, math::float3 const& d_ir, float max_distance, float& distance ) const
{ 
	return collision::terrain_ray_test( m_data, p_os, d_ir, max_distance, distance );
}



void terrain_geometry::add_triangles( collision::triangles_type& )const
{ NOT_IMPLEMENTED(); }

void terrain_geometry::generate_contacts( collision::on_contact&, const float4x4&, const float4x4&, const collision::sphere_geometry& )const
{ NOT_IMPLEMENTED(); }

void terrain_geometry::generate_contacts( collision::on_contact&, const float4x4&, const float4x4&, const collision::geometry& )const
{ NOT_IMPLEMENTED(); }

void terrain_geometry::generate_contacts( collision::on_contact&, const float4x4&, const float4x4&, const collision::box_geometry& )const
{ NOT_IMPLEMENTED(); }

void terrain_geometry::generate_contacts( collision::on_contact&, const float4x4&, const float4x4&, const collision::cylinder_geometry& )const
{ NOT_IMPLEMENTED(); }

void terrain_geometry::generate_contacts( collision::on_contact&, const float4x4&, const float4x4&, const collision::triangle_mesh_base& ) const
{ NOT_IMPLEMENTED(); }

void terrain_geometry::generate_contacts( collision::on_contact&, const float4x4 &, const float4x4&, const collision::composite_geometry& )const
{ 
	NOT_IMPLEMENTED(); 
}


} // namespace collision
} // namespace xray
