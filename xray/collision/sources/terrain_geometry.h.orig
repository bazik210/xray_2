////////////////////////////////////////////////////////////////////////////
//	Created		: 01.09.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef TERRAIN_GEOMETRY_H_INCLUDED
#define TERRAIN_GEOMETRY_H_INCLUDED

#include <xray/collision/geometry.h>

namespace xray {
namespace collision {

struct terrain_data
{
	 
					terrain_data	(  float phisical_size, u32 vertex_row_size, const float* heightfield   );
	
inline		float	quad_size		( )const;	
inline	 	float	min_height		( )const;	
inline	 	float	max_height		( )const;	
inline	 	u32		quad_dimension	( )const;	
inline		u32		vert_row_size	( )const	{ return m_vertex_row_size; }
inline		float	physical_size	( )const	{ return m_physical_size; }
inline	 	bool	get_row_col		( float3 const& position_local, int& x, int& z )const ;

			bool	ray_test_quad	(	int const y1, int const x1, 
										float3 const& ray_point, 
										float3 const& ray_dir,
										float const max_distance,
										float& range,
										bool log_out ) const;

inline	 	float3	position			(u16 vertex_id) const;


float				m_physical_size;
u32					m_vertex_row_size;
const float			*m_heightfield;

};




class terrain_geometry :	public collision::geometry,
					   		private boost::noncopyable

{

public:
						terrain_geometry	( float phisical_size, u32 vertex_row_size, const float* heightfield );
	virtual				~terrain_geometry	( );

	virtual	void		destroy				( memory::base_allocator* allocator );

	virtual	math::aabb&	get_aabb			( math::aabb& result ) const;
	virtual	float3		get_random_surface_point( math::random32& randomizer ) const;
	virtual	float		get_surface_area	( ) const;

	virtual void		accept				( geometry_double_dispatcher& dispatcher, geometry const& node ) const;
	virtual void		visit				( geometry_double_dispatcher& dispatcher, box_geometry const& node ) const;
	virtual void		visit				( geometry_double_dispatcher& dispatcher, sphere_geometry const& node ) const;
	virtual void		visit				( geometry_double_dispatcher& dispatcher, cylinder_geometry const& node ) const;

private:
	virtual	void		render				( render::scene_ptr const& scene, render::debug::renderer& , float4x4 const& ) const;
	virtual	math::float3 const* vertices	( ) const;
	virtual	u32			vertex_count		( ) const;
	virtual	u32 const*	indices				( ) const;
	virtual	u32 const*	indices				( u32 triangle_id ) const;
	virtual	u32			index_count			( )	const;
	virtual	bool		aabb_query			( collision::object const* object, math::aabb const& aabb, collision::triangles_type& triangles )		const;
	virtual	bool		cuboid_query		( collision::object const* object, math::cuboid const& cuboid, collision::triangles_type& triangles )	const;

	virtual	bool		ray_query			(
							collision::object const* object,
							float3 const& pos,
							float3 const& dir,
							float max_distance,
							float& distance,
							collision::ray_triangles_type& triangles,
							collision::triangles_predicate_type const& predicate ) const;

	virtual	bool		aabb_test			( math::aabb const& aabb ) const;
	virtual	bool		cuboid_test			( math::cuboid const& cuboid ) const;
	virtual	bool		ray_test			( math::float3 const& origin, math::float3 const& direction, float max_distance, float& distance ) const;
	virtual	void		add_triangles		( collision::triangles_type& triangles ) const;

public:
	virtual	void		enumerate_primitives( enumerate_primitives_callback&  ) const {};
	virtual	void		enumerate_primitives( float4x4 const&, enumerate_primitives_callback& ) const {} ;				

private:
	virtual	void		generate_contacts	( collision::on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const collision::sphere_geometry& og )			const;
	virtual	void		generate_contacts	( collision::on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const collision::geometry& og )					const;
	virtual	void		generate_contacts	( collision::on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const collision::box_geometry& og )				const;
	virtual	void		generate_contacts	( collision::on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const collision::cylinder_geometry& og )			const;
	virtual	void		generate_contacts	( collision::on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const collision::triangle_mesh_base& og )		const;
	virtual	void		generate_contacts	( collision::on_contact& c, const float4x4 &self_transform, const float4x4 &transform, const collision::composite_geometry& og )			const;

private:
	terrain_data							m_data;
}; // class terrain_geometry

} // namespace collision
} // namespace xray

#endif // #ifndef TERRAIN_GEOMETRY_H_INCLUDED