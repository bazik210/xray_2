////////////////////////////////////////////////////////////////////////////
//	Created		: 06.09.2011
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "capsule_geometry.h"
#include <xray/collision/contact_info.h>
#include <xray/math_randoms_generator.h>
#include <xray/render/facade/debug_renderer.h>

namespace xray {
namespace collision {

bool segment_segment_intersect ( float3 const& p1, float3 const& p2, float3 const& p3, float3 const& p4, float max_distance );

capsule_geometry::capsule_geometry	( float const radius, float const half_length ) :
	m_radius						( radius ),
	m_half_length					( half_length )
{
}

void	capsule_geometry::destroy		( memory::base_allocator* allocator )
{
	XRAY_UNREFERENCED_PARAMETER		( allocator );
}

void	capsule_geometry::add_triangles		( triangles_type& triangles ) const
{
	XRAY_UNREFERENCED_PARAMETER		(triangles);
	UNREACHABLE_CODE();
}

void	capsule_geometry::generate_contacts	( on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const cylinder_geometry& og ) const 
{
	XRAY_UNREFERENCED_PARAMETERS	(&c, &self_transform, &transform, &og);
}

void	capsule_geometry::generate_contacts	( on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const triangle_mesh_base& og ) const 
{
	XRAY_UNREFERENCED_PARAMETERS	(&c, &self_transform, &transform, &og);
}

void	capsule_geometry::generate_contacts	( on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const composite_geometry& og )const
{
	XRAY_UNREFERENCED_PARAMETERS	( &c, &self_transform, &transform, &og );
}

void	capsule_geometry::generate_contacts( on_contact& on_c, const float4x4 &self_transform, const float4x4 &pose, const sphere_geometry& og )const
{
	XRAY_UNREFERENCED_PARAMETERS	( &on_c, &self_transform, &pose, &og );
	NOT_IMPLEMENTED					( );
}

bool capsule_geometry::aabb_query	( object const* object, math::aabb const& aabb, triangles_type& triangles ) const 
{
	XRAY_UNREFERENCED_PARAMETERS	(object, &aabb, &triangles);
	NOT_IMPLEMENTED					( return false );
}

float3 capsule_geometry::get_random_surface_point( math::random32& randomizer ) const
{
	XRAY_UNREFERENCED_PARAMETERS	( randomizer );
	NOT_IMPLEMENTED					( return float3(0, 0, 0) );
}

float capsule_geometry::get_surface_area	( ) const
{
	NOT_IMPLEMENTED					( return 0 );
}

bool capsule_geometry::cuboid_query	( object const* object, math::cuboid const& cuboid, triangles_type& triangles ) const
{
	XRAY_UNREFERENCED_PARAMETERS	(object, &cuboid, &triangles);
	NOT_IMPLEMENTED					( return false );
}

bool capsule_geometry::ray_query		( object const* object,
									  float3 const& origin,
									  float3 const& direction,
									  float max_distance,
									  float& distance,
									  ray_triangles_type& triangles,
									  triangles_predicate_type const& predicate ) const				
{
	XRAY_UNREFERENCED_PARAMETERS	( object, &origin, &direction, max_distance, distance, &triangles, &predicate );
	return false;
}

bool capsule_geometry::aabb_test	( math::aabb const& aabb ) const
{
	XRAY_UNREFERENCED_PARAMETER		( aabb );
	NOT_IMPLEMENTED					( return false );
}

bool capsule_geometry::cuboid_test	( math::cuboid const& cuboid ) const
{
	XRAY_UNREFERENCED_PARAMETER		( cuboid );
	NOT_IMPLEMENTED					( return false );
}

bool capsule_geometry::ray_test		( math::float3 const& origin, math::float3 const& direction, float max_distance, float& distance ) const
{
	return segment_segment_intersect(
		float3( 0, -1, 0 ) * m_half_length,
		float3( 0, 1, 0 ) * m_half_length,
		origin,
		origin + direction * max_distance,
		m_radius
	);
}

void	capsule_geometry::generate_contacts(  on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const collision::geometry& og )		const
{
	XRAY_UNREFERENCED_PARAMETERS	( c, self_transform, transform, og );
	NOT_IMPLEMENTED					( );
}

void	capsule_geometry::generate_contacts	( on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const box_geometry& )				const 
{
	XRAY_UNREFERENCED_PARAMETERS	( c, self_transform, transform );
	NOT_IMPLEMENTED					( );
}

void	capsule_geometry::render( render::scene_ptr const& scene, render::debug::renderer& renderer, float4x4 const& matrix ) const 
{
	renderer.draw_line_capsule( scene, matrix, float3( m_radius, m_half_length, m_radius ), math::color( 255u, 255u, 255u, 255u ) );
}

math::aabb& capsule_geometry::get_aabb	( math::aabb& result ) const
{
	result				= 
		math::create_aabb_min_max(
			float3( -m_radius, -m_half_length-m_radius, -m_radius ),
			float3( +m_radius, +m_half_length+m_radius, +m_radius )
		);
	return				result;
}

void capsule_geometry::get_aabb			( math::aabb& bbox, float4x4 const& self_transform ) const
{
	float3 size							= float3( self_transform.j.xyz() * half_length() + m_radius ).abs() +  
										( float3( self_transform.i.xyz() ).abs() + 
										  float3( self_transform.k.xyz() ).abs() ) * radius();

	bbox								= math::create_aabb_min_max( self_transform.c.xyz() - size, self_transform.c.xyz() + size );
}

math::float3 const* capsule_geometry::vertices	( ) const
{
	NOT_IMPLEMENTED(return 0);
}

u32		capsule_geometry::vertex_count				( ) const
{
	NOT_IMPLEMENTED(return 0);
}

u32 const* capsule_geometry::indices				( ) const
{
	NOT_IMPLEMENTED(return 0);
}

u32 const* capsule_geometry::indices				( u32 triangle_id ) const
{
	XRAY_UNREFERENCED_PARAMETER		( triangle_id );
	NOT_IMPLEMENTED(return 0);
}

u32		capsule_geometry::index_count				( ) const
{
	NOT_IMPLEMENTED(return 0);
}

void	capsule_geometry::enumerate_primitives	( enumerate_primitives_callback& cb ) const
{
	cb.enumerate( float4x4().identity(), primitive( capsule( m_half_length, m_radius ) ) );
}

void	capsule_geometry::enumerate_primitives	( float4x4 const& transform, enumerate_primitives_callback& cb ) const
{
	cb.enumerate( transform, primitive( capsule( m_half_length, m_radius ) ) );
}

	
} // namespace collision
} // namespace xray