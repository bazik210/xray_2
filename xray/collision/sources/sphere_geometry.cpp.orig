////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.02.2008
//	Author		: Konstantin Slipchenko
//	Description : geometry: box geometry
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "sphere_geometry.h"
#include "box_geometry.h"
#include "cylinder_geometry.h"
#include "composite_geometry.h"
#include "triangle_mesh_base.h"
#include <xray/collision/geometry_double_dispatcher.h>
#include <xray/collision/contact_info.h>
#include <xray/render/facade/debug_renderer.h>
#include "xray/math_randoms_generator.h"

//void	sphere_geometry::generate_contacts( contact_info_buffer& contacts, const pose_anchor &anch, const geometry& og )const
//{
//	og.generate_contacts(contacts, anch, *this );
//}

namespace xray {
namespace collision {

sphere_geometry::sphere_geometry	( float const radius ) :
	m_radius						( radius )
{
}

void sphere_geometry::destroy		( memory::base_allocator* allocator )
{
	XRAY_UNREFERENCED_PARAMETER		( allocator );
}

void sphere_geometry::add_triangles		( triangles_type& triangles ) const
{
	XRAY_UNREFERENCED_PARAMETER		(triangles);
	UNREACHABLE_CODE();
}

void sphere_geometry::accept	( geometry_double_dispatcher& dispatcher, geometry const& node ) const
{
	node.visit			( dispatcher, *this );
}

void sphere_geometry::visit		( geometry_double_dispatcher& dispatcher, box_geometry const& node ) const
{
	dispatcher.dispatch	( node, *this );
}

void sphere_geometry::visit		( geometry_double_dispatcher& dispatcher, sphere_geometry const& node ) const
{
	dispatcher.dispatch	( node, *this );
}

void sphere_geometry::visit		( geometry_double_dispatcher& dispatcher, cylinder_geometry const& node ) const
{
	dispatcher.dispatch	( node, *this );
}

void	sphere_geometry::generate_contacts	( on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const cylinder_geometry& og ) const 
{
	c.change_order( ); 
	og.generate_contacts( c, transform, self_transform, *this ); //change order
	c.change_order( ); 
}

void	sphere_geometry::generate_contacts	( on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const triangle_mesh_base& og ) const 
{
	XRAY_UNREFERENCED_PARAMETERS	(&c, &self_transform, &transform, &og);
	c.change_order( ); 
	og.generate_contacts( c, transform, self_transform, *this ); //change order
	c.change_order( ); 
	//	t_generate_contact_primitive<detail::triangle_sphere_contact_tests>( c, self_transform,  transform, og );
}

void	sphere_geometry::generate_contacts	( on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const composite_geometry& og )const
{
	XRAY_UNREFERENCED_PARAMETERS	(&c, &self_transform, &transform, &og);
}

void	sphere_geometry::generate_contacts( on_contact& on_c, const float4x4 &self_transform, const float4x4 &pose, const sphere_geometry& og )const
{
	const float3 replacement		= pose.c.xyz() - self_transform.c.xyz();
	
	const float square_distance		= replacement.square_length();
	const float radius_sum			= radius() + og.radius();
	const float square_radius_sum	= radius_sum * radius_sum;
	if( square_distance > square_radius_sum )
		return;

	if( square_distance < xray::math::epsilon_7 )
	{
		contact_info c;
		c.depth = xray::math::max( radius(), og.radius() );
		c.normal.set( 0,-1,0);
		c.position =  self_transform.c.xyz();
		on_c( c );
		return;
	}
	
	const float distance			= sqrt( square_distance );

	contact_info c;
	c.depth		= radius_sum - distance ;
	c.normal	= -replacement/distance;
	c.position	= self_transform.c.xyz() - c.normal * radius();
	on_c( c );
}

bool sphere_geometry::aabb_query	( object const* object, math::aabb const& aabb, triangles_type& triangles ) const 
{
	XRAY_UNREFERENCED_PARAMETERS	(object, &aabb, &triangles);
	return false; 
}

float3 sphere_geometry::get_random_surface_point( math::random32& randomizer ) const
{
	float const z_random		= -m_radius + randomizer.random_f( 2.f * m_radius );
	float const teta_angle		= randomizer.random_f( math::pi_x2 );
	float const r_coefficient	= math::sqrt( math::sqr( m_radius ) - math::sqr( z_random ) );
	float const x_random		= r_coefficient * math::cos( teta_angle );
 	float const y_random		= r_coefficient * math::sin( teta_angle );

	return						float3( x_random, y_random, z_random );
}

float sphere_geometry::get_surface_area	( ) const
{
	return 4 * math::pi * math::sqr( m_radius );
}

bool sphere_geometry::cuboid_query	( object const* object, math::cuboid const& cuboid, triangles_type& triangles ) const
{
	XRAY_UNREFERENCED_PARAMETERS	(object, &cuboid, &triangles);
	return false; 
}

bool sphere_geometry::ray_query		( object const* object,
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

bool sphere_geometry::aabb_test		( math::aabb const& aabb ) const
{
	XRAY_UNREFERENCED_PARAMETER		( aabb );
	return true;
}

bool sphere_geometry::cuboid_test	( math::cuboid const& cuboid ) const
{
	XRAY_UNREFERENCED_PARAMETER		( cuboid );
	return true;
}

bool sphere_geometry::ray_test		( math::float3 const& origin, math::float3 const& direction, float max_distance, float& distance ) const
{
	XRAY_UNREFERENCED_PARAMETER		( max_distance );
	float3 const proj				=  (-origin | direction) * direction;
	distance						= length(proj);
	return							length( proj - (-origin) ) <= m_radius;
}

//void	sphere_geometry::generate_contacts( contact_info_buffer& contacts, const pose_anchor &anch, const transformed_geometry& og )const
//{
//
//}
//
//void	sphere_geometry::generate_contacts( contact_info_buffer& contacts, const pose_anchor &anch, const complex_geometry& og )const
//{
//
//}
//
//
//void	sphere_geometry::generate_contacts( contact_info_buffer& contacts, const pose_anchor &anch, const box_geometry& og )const
//{
//
//}
//
//
//
//void	sphere_geometry::generate_contacts( contact_info_buffer& contacts, const pose_anchor &anch, const sphere_geometry& og )const
//{
//
//}
//
//void	sphere_geometry::generate_contacts( contact_info_buffer& contacts, const pose_anchor &anch, const triangle_mesh_base& og )	const
//{
//
//}

void	sphere_geometry::generate_contacts(  on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const collision::geometry& og )		const
{
	c.change_order( ); 
	og.generate_contacts( c, transform, self_transform, *this ); //change order
	c.change_order( ); 
}

void	sphere_geometry::generate_contacts	( on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const box_geometry& og )				const 
{
	c.change_order( ); 
	og.generate_contacts( c, transform, self_transform, *this ); //change order
	c.change_order( );
}

void	sphere_geometry::render( render::scene_ptr const& scene, render::debug::renderer& renderer, float4x4 const& matrix ) const 
{
	renderer.draw_sphere( scene, matrix.c.xyz(), m_radius, math::color( 255u, 255u, 255u, 255u ) );
}

math::aabb& sphere_geometry::get_aabb	( math::aabb& result ) const
{
	result				= 
		math::create_aabb_min_max(
			float3( -m_radius, -m_radius, -m_radius ),
			float3( +m_radius, +m_radius, +m_radius )
		);
	return				result;
}

math::float3 const* sphere_geometry::vertices	( ) const
{
	NOT_IMPLEMENTED(return 0);
}

u32 sphere_geometry::vertex_count				( ) const
{
	NOT_IMPLEMENTED(return 0);
}

u32 const* sphere_geometry::indices				( ) const
{
	NOT_IMPLEMENTED(return 0);
}

u32 const* sphere_geometry::indices				( u32 triangle_id ) const
{
	XRAY_UNREFERENCED_PARAMETER		( triangle_id );
	NOT_IMPLEMENTED(return 0);
}

u32 sphere_geometry::index_count				( ) const
{
	NOT_IMPLEMENTED(return 0);
}

void		sphere_geometry::enumerate_primitives	( enumerate_primitives_callback& cb ) const
{
	cb.enumerate( float4x4().identity(), primitive( sphere( m_radius ) ) );
}

void		sphere_geometry::enumerate_primitives	( float4x4 const& transform, enumerate_primitives_callback& cb ) const
{
	cb.enumerate( transform, primitive( sphere( m_radius ) ) );
}


} // namespace collision
} // namespace xray