////////////////////////////////////////////////////////////////////////////
//	Created		: 30.08.2011
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "containment_check_double_dispatcher.h"
#include "box_geometry.h"
#include "sphere_geometry.h"
#include "cylinder_geometry.h"
#include <xray/collision/geometry.h>
#include <xray/collision/geometry_instance.h>

namespace xray{
namespace collision	{

containment_check_double_dispatcher::containment_check_double_dispatcher(
				geometry_instance const& bounding_volume,
				geometry_instance const& testee
			):
				m_bounding_volume	( bounding_volume ),
				m_testee			( testee ),
				m_result			( false )
#ifndef MASTER_GOLD
				,
				m_renderer			( NULL ),
				m_scene				( NULL ) 
#endif
				{ }

#ifndef MASTER_GOLD
containment_check_double_dispatcher::containment_check_double_dispatcher(
				geometry_instance const& bounding_volume,
				geometry_instance const& testee,
				render::debug::renderer* renderer,
				render::scene_ptr const* scene
			):
				m_bounding_volume	( bounding_volume ),
				m_testee			( testee ),
				m_result			( false ),
				m_renderer			( renderer ),
				m_scene				( scene ) { }
#endif

bool containment_check_double_dispatcher::check_contains ( )
{
	geometry const* bounding_volume	= &*m_bounding_volume.get_geometry( );
	geometry const* testee			= &*m_testee.get_geometry( );

	bounding_volume->accept( *this, *testee );
	//bounding_volume->accept( *this, *testee );
	return m_result;
}

void containment_check_double_dispatcher::dispatch	( geometry const& bounding_volume, geometry const& testee )
{
	XRAY_UNREFERENCED_PARAMETERS( testee, bounding_volume );
	NOT_IMPLEMENTED( );
}

////////////////		B O X   T E S T S			////////////////

static float3 const cuboid_vertices[] = {
	float3( -1.f, -1.f, -1.f ),
	float3( -1.f, -1.f, +1.f ),
	float3( -1.f, +1.f, -1.f ),
	float3( -1.f, +1.f, +1.f ),
	float3( +1.f, -1.f, -1.f ),
	float3( +1.f, -1.f, +1.f ),
	float3( +1.f, +1.f, -1.f ),
	float3( +1.f, +1.f, +1.f ),
};

static inline bool is_inside_obb ( float3 const& vertex, float3 const& obb, float4x4 const& obb_transform )
{
	float3 const& center_to_vertex	= vertex - obb_transform.c.xyz( );
	float const projection_on_x		= math::abs( center_to_vertex | obb_transform.i.xyz( ) );
	if ( projection_on_x > obb.x )
		return					false;

	float const projection_on_y		= math::abs( center_to_vertex | obb_transform.j.xyz( ) );
	if ( projection_on_y > obb.y )
		return					false;

	float const projection_on_z		= math::abs( center_to_vertex | obb_transform.k.xyz( ) );
	if ( projection_on_z > obb.z )
		return					false;

	return						true;
}

void containment_check_double_dispatcher::dispatch	( box_geometry const& bounding_volume, box_geometry const& testee )
{
	float4x4 matrix( m_testee.get_matrix( ) );
	m_testee.get_matrix( );
	matrix.i *= testee.half_sides( ).x;
	matrix.j *= testee.half_sides( ).y;
	matrix.k *= testee.half_sides( ).z;

	for ( u32 i=0; i < array_size( cuboid_vertices ); ++i )
	{
		float3 const& vertex	= matrix.transform_position( cuboid_vertices[i] );
		if( !is_inside_obb( vertex, bounding_volume.half_sides( ), m_bounding_volume.get_matrix( ) ) )
			return;

#ifndef MASTER_GOLD
		m_renderer->draw_sphere_solid	( *m_scene, vertex, 0.2f, math::color( 0, 0, 255, 255 ) );
#endif //#ifndef MASTER_GOLD

	}

	m_result = true;
}

static inline bool is_inside_sphere ( float3 const& vertex, float3 const& sphere_center, float const squared_sphere_radius )
{
	return  ( vertex - sphere_center ).square_length( ) <= squared_sphere_radius;
}

void containment_check_double_dispatcher::dispatch	( sphere_geometry const& bounding_volume, box_geometry const& testee )
{
	float4x4 matrix( m_testee.get_matrix( ) );
	matrix.i *= testee.half_sides( ).x;
	matrix.j *= testee.half_sides( ).y;
	matrix.k *= testee.half_sides( ).z;

	float const squared_sphere_radius = math::sqr( bounding_volume.radius( ) );

	for ( u32 i=0; i < array_size( cuboid_vertices ); ++i )
	{
		float3 const& vertex		= matrix.transform_position( cuboid_vertices[i] );
		if( !is_inside_sphere( vertex, m_bounding_volume.get_matrix( ).c.xyz( ), squared_sphere_radius ) )
			return;

#ifndef MASTER_GOLD
		m_renderer->draw_sphere_solid	( *m_scene, vertex, 0.2f, math::color( 0, 0, 255, 255 ) );
#endif //#ifndef MASTER_GOLD
	}

	m_result = true;
}

static inline bool is_inside_cylinder ( float3 const& vertex, float const cylinder_half_height, float const squared_cylinder_radius, float4x4 const& cylinder_transform )
{
	float3 const& center_to_vertex	= vertex - cylinder_transform.c.xyz( );
	float const projection_on_axe	= center_to_vertex | cylinder_transform.j.xyz( );

	if ( math::abs( projection_on_axe ) > cylinder_half_height )
		return					false;

	float3 const& projection_to_vertex	= cylinder_transform.j.xyz( ) * projection_on_axe - center_to_vertex;
	return						projection_to_vertex.square_length( ) <= squared_cylinder_radius;
}

void containment_check_double_dispatcher::dispatch	( cylinder_geometry const& bounding_volume, box_geometry const& testee )
{
	float4x4 matrix( m_testee.get_matrix( ) );
	matrix.i *= testee.half_sides( ).x;
	matrix.j *= testee.half_sides( ).y;
	matrix.k *= testee.half_sides( ).z;

	float const squared_cylinder_radius = math::sqr( bounding_volume.radius( ) );

	for ( u32 i=0; i < array_size( cuboid_vertices ); ++i )
	{
		float3 const& vertex		= matrix.transform_position( cuboid_vertices[i] );
		if( !is_inside_cylinder( vertex, bounding_volume.half_length( ), squared_cylinder_radius, m_bounding_volume.get_matrix( ) ) )
			return;

#ifndef MASTER_GOLD
		m_renderer->draw_sphere_solid	( *m_scene, vertex, 0.2f, math::color( 0, 0, 255, 255 ) );
#endif //#ifndef MASTER_GOLD
	}

	m_result = true;
}

////////////////		S P H E R E   T E S T S			////////////////

void containment_check_double_dispatcher::dispatch	( box_geometry const& bounding_volume, sphere_geometry const& testee )
{
	float3 const& center_to_sphere_center	= m_testee.get_matrix( ).c.xyz( ) - m_bounding_volume.get_matrix( ).c.xyz( );
	float const projection_on_x				= math::abs( center_to_sphere_center | m_bounding_volume.get_matrix( ).i.xyz( ) );
	if ( projection_on_x + testee.radius( ) > bounding_volume.half_sides( ).x )
		return;

	float const projection_on_y				= math::abs( center_to_sphere_center | m_bounding_volume.get_matrix( ).j.xyz( ) );
	if ( projection_on_y + testee.radius( ) > bounding_volume.half_sides( ).y )
		return;

	float const projection_on_z				= math::abs( center_to_sphere_center | m_bounding_volume.get_matrix( ).k.xyz( ) );
	if ( projection_on_z + testee.radius( ) > bounding_volume.half_sides( ).z )
		return;

	m_result = true;
}
void containment_check_double_dispatcher::dispatch	( sphere_geometry const& bounding_volume, sphere_geometry const& testee )
{
	if( testee.radius( ) > bounding_volume.radius( ) )
		return;

	m_result = ( m_testee.get_matrix( ).c.xyz( ) - m_bounding_volume.get_matrix( ).c.xyz( ) ).square_length( ) <= math::sqr( bounding_volume.radius( ) - testee.radius( ) );
}
void containment_check_double_dispatcher::dispatch	( cylinder_geometry const& bounding_volume, sphere_geometry const& testee )
{
	if( testee.radius( ) > bounding_volume.radius( ) )
		return;

	float3 const& center_to_sphere_center	= m_testee.get_matrix( ).c.xyz( ) - m_bounding_volume.get_matrix( ).c.xyz( );
	float const projection_on_axe			= center_to_sphere_center | m_bounding_volume.get_matrix( ).j.xyz( );

	if ( math::abs( projection_on_axe ) + testee.radius( ) > bounding_volume.half_length( ) )
		return;

	float3 const& projection_to_sphere_center	= m_bounding_volume.get_matrix( ).j.xyz( ) * projection_on_axe - center_to_sphere_center;
	m_result =	projection_to_sphere_center.square_length( ) <= math::sqr( bounding_volume.radius( ) - testee.radius( ) );
}

////////////////		C Y L I N D E R   T E S T S			////////////////

static inline bool is_inside_range(	float3 const&	test_cylinder_relative_position,
									float3 const&	test_cylinder_center,
									float3 const&	test_cylinder_center_to_cap_vector,
									float3 const&	test_cylinder_up_axis,
									float3 const&	test_cylinder_alternative_axis,
									float const		test_cylinder_radius,
									float3 const&	range_axis,
									float const		range_half_size,
									float3 const&	range_center
#ifndef MASTER_GOLD
									,
									render::debug::renderer &	renderer,
									render::scene_ptr const&	scene,
									math::color					debug_color
#endif
								)
{
	float3			to_extremums_axis;
	float3			cross_product			= test_cylinder_up_axis ^ range_axis;

	if( math::is_zero( cross_product.x + cross_product.y + cross_product.z ) )
	{
		to_extremums_axis = test_cylinder_alternative_axis;
	}
	else
	{
		to_extremums_axis = test_cylinder_up_axis ^ cross_product;
		to_extremums_axis.normalize( );
	}
	
	float3 cylinder_edge_center;

	if( ( test_cylinder_relative_position | range_axis ) > 0 )
		cylinder_edge_center			= test_cylinder_center - to_extremums_axis * test_cylinder_radius;
	else
		cylinder_edge_center			= test_cylinder_center + to_extremums_axis * test_cylinder_radius;

	float3	vertex;
	float	projection_on_axe;

	vertex				= cylinder_edge_center + test_cylinder_center_to_cap_vector;
	projection_on_axe	= ( vertex - range_center ) | range_axis;
	if( math::abs( projection_on_axe ) > range_half_size )
		return					false;

#ifndef MASTER_GOLD
	renderer.draw_sphere_solid	( scene, vertex, 0.2f, debug_color );
#endif

	vertex				= cylinder_edge_center - test_cylinder_center_to_cap_vector;
	projection_on_axe	= ( vertex - range_center ) | range_axis;
	if( math::abs( projection_on_axe ) > range_half_size )
		return					false;

#ifndef MASTER_GOLD
	renderer.draw_sphere_solid	( scene, vertex, 0.2f, debug_color );
#endif

	return true;
}

void containment_check_double_dispatcher::dispatch	( box_geometry const& bounding_volume, cylinder_geometry const& testee )
{
	float3 const	test_cylinder_relative_position = m_testee.get_matrix( ).c.xyz( ) - m_bounding_volume.get_matrix( ).c.xyz( );
	float3 const	center_to_cup_vector			= m_testee.get_matrix( ).j.xyz( ) * testee.half_length( );
	
	//x axis test
	if( !is_inside_range	(	test_cylinder_relative_position,
							m_testee.get_matrix( ).c.xyz( ),
							center_to_cup_vector,
							m_testee.get_matrix( ).j.xyz( ),
							m_testee.get_matrix( ).i.xyz( ),
							testee.radius( ),
							m_bounding_volume.get_matrix( ).i.xyz( ),
							bounding_volume.half_sides( ).x,
							m_bounding_volume.get_matrix( ).c.xyz( )
#ifndef MASTER_GOLD
							,
							*m_renderer,
							*m_scene,
							math::color( 200, 0, 0, 255 ) 
#endif //#ifndef MASTER_GOLD
						) )
		return;

	//z axis test
	if( !is_inside_range	(	test_cylinder_relative_position,
							m_testee.get_matrix( ).c.xyz( ),
							center_to_cup_vector,
							m_testee.get_matrix( ).j.xyz( ),
							m_testee.get_matrix( ).i.xyz( ),
							testee.radius( ),
							m_bounding_volume.get_matrix( ).k.xyz( ),
							bounding_volume.half_sides( ).z,
							m_bounding_volume.get_matrix( ).c.xyz( )
#ifndef MASTER_GOLD
							,
							*m_renderer,
							*m_scene,
							math::color( 0, 0, 200, 255 ) 
#endif //#ifndef MASTER_GOLD
						) )
		return;

	//y axis test
	if( !is_inside_range	(	test_cylinder_relative_position,
							m_testee.get_matrix( ).c.xyz( ),
							center_to_cup_vector,
							m_testee.get_matrix( ).j.xyz( ),
							m_testee.get_matrix( ).i.xyz( ),
							testee.radius( ),
							m_bounding_volume.get_matrix( ).j.xyz( ),
							bounding_volume.half_sides( ).y,
							m_bounding_volume.get_matrix( ).c.xyz( )
#ifndef MASTER_GOLD
							,
							*m_renderer,
							*m_scene,
							math::color( 0, 128, 0, 255 ) 
#endif //#ifndef MASTER_GOLD
						) )
		return;

	m_result = true;
}
void containment_check_double_dispatcher::dispatch	( sphere_geometry const& bounding_volume, cylinder_geometry const& testee )
{
	float3 const&	test_cylinder_center		= m_testee.get_matrix( ).c.xyz( );
	float3 const&	cylinder_center_to_center	= m_bounding_volume.get_matrix( ).c.xyz( ) - m_testee.get_matrix( ).c.xyz( ); 
	float3 const	cross_product				= m_testee.get_matrix( ).j.xyz( ) ^ cylinder_center_to_center;
	float3			cylinder_x_axe;
	
	if( cross_product.x + cross_product.y + cross_product.z == 0 )
	{
		cylinder_x_axe = m_testee.get_matrix( ).i.xyz( );
	}
	else
	{
		cylinder_x_axe = m_testee.get_matrix( ).j.xyz( ) ^ cross_product;
		cylinder_x_axe.normalize( );
	}

	float3 far_cylinder_edge_center		= test_cylinder_center + cylinder_x_axe * testee.radius( );
	float3 center_to_cup_vector			= m_testee.get_matrix( ).j.xyz( ) * testee.half_length( );
	float3 vertex;

	vertex = far_cylinder_edge_center + center_to_cup_vector;
	if ( ( vertex - m_bounding_volume.get_matrix( ).c.xyz( ) ).square_length( ) > math::sqr( bounding_volume.radius( ) ) )
		return;

#ifndef MASTER_GOLD
		m_renderer->draw_sphere_solid	( *m_scene, vertex, 0.2f, math::color( 0, 0, 255, 255 ) );
#endif //#ifndef MASTER_GOLD

	vertex = far_cylinder_edge_center - center_to_cup_vector;
	if ( ( vertex - m_bounding_volume.get_matrix( ).c.xyz( ) ).square_length( ) > math::sqr( bounding_volume.radius( ) ) )
		return;

#ifndef MASTER_GOLD
		m_renderer->draw_sphere_solid	( *m_scene, vertex, 0.2f, math::color( 0, 0, 255, 255 ) );
#endif //#ifndef MASTER_GOLD

	m_result = true;
}
void containment_check_double_dispatcher::dispatch	( cylinder_geometry const& bounding_volume, cylinder_geometry const& testee )
{
	float3 const&	test_cylinder_center			= m_testee.get_matrix( ).c.xyz( );
	float3 const&	cylinder_center					= m_bounding_volume.get_matrix( ).c.xyz( );
	float3			center_to_cup_vector			= m_testee.get_matrix( ).j.xyz( ) * testee.half_length( );
	
	float3			cylinder_edge_center;
	float3			vertex;
	float			projection_on_axe;
	float3			projected_vertex;
	float3			cross_product;

	//vertical test
	if ( !is_inside_range(	m_testee.get_matrix( ).c.xyz( ) - m_bounding_volume.get_matrix( ).c.xyz( ),
							m_testee.get_matrix( ).c.xyz( ),
							center_to_cup_vector,
							m_testee.get_matrix( ).j.xyz( ),
							m_testee.get_matrix( ).i.xyz( ),
							testee.radius( ),
							m_bounding_volume.get_matrix( ).j.xyz( ),
							bounding_volume.half_length( ),
							m_bounding_volume.get_matrix( ).c.xyz( )
#ifndef MASTER_GOLD
							,
							*m_renderer,
							*m_scene,
							math::color( 0, 128, 0, 255 )  
#endif //#ifndef MASTER_GOLD
						) )
		return;
	
	//horizontal test
	float3 const	projected_test_cylinder_center	= cylinder_center + ( ( test_cylinder_center - cylinder_center ) | m_bounding_volume.get_matrix( ).j.xyz( ) ) * m_bounding_volume.get_matrix( ).j.xyz( );
	float3			cylinder_center_to_center		= projected_test_cylinder_center - test_cylinder_center;
					cross_product					= m_testee.get_matrix( ).j.xyz( ) ^ cylinder_center_to_center;
	float3			cylinder_x_axe;
	
	if( math::is_zero( cross_product.x + cross_product.y + cross_product.z ) )
	{
		cylinder_x_axe = m_testee.get_matrix( ).j.xyz( ) ^ m_bounding_volume.get_matrix( ).j.xyz( );
	}
	else
	{
		cylinder_x_axe = m_testee.get_matrix( ).j.xyz( ) ^ cross_product;
		cylinder_x_axe.normalize( );
	}

	cylinder_edge_center			= test_cylinder_center + cylinder_x_axe * testee.radius( );
		
	vertex				= cylinder_edge_center + center_to_cup_vector;
	projection_on_axe	= ( vertex - cylinder_center ) | m_bounding_volume.get_matrix( ).j.xyz( );
	projected_vertex	= cylinder_center + projection_on_axe * m_bounding_volume.get_matrix( ).j.xyz( );
	
	if ( ( vertex - projected_vertex ).square_length( ) > math::sqr( bounding_volume.radius( ) ) )
		return;

#ifndef MASTER_GOLD
		m_renderer->draw_sphere_solid	( *m_scene, vertex, 0.2f, math::color( 0, 0, 255, 255 ) );
#endif //#ifndef MASTER_GOLD

	vertex				= cylinder_edge_center - center_to_cup_vector;
	projection_on_axe	= ( vertex - cylinder_center ) | m_bounding_volume.get_matrix( ).j.xyz( );
	projected_vertex	= cylinder_center + projection_on_axe * m_bounding_volume.get_matrix( ).j.xyz( );

	if ( ( vertex - projected_vertex ).square_length( ) > math::sqr( bounding_volume.radius( ) ) )
		return;

#ifndef MASTER_GOLD
		m_renderer->draw_sphere_solid	( *m_scene, vertex, 0.2f, math::color( 0, 0, 255, 255 ) );
#endif //#ifndef MASTER_GOLD

	m_result = true;
}

} // namespace collision
} // namespace xray
