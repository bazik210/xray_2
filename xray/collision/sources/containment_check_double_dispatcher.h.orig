////////////////////////////////////////////////////////////////////////////
//	Created		: 30.08.2011
//	Author		: Evgeniy Obertyukh
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_COLLISION_CONTAINMENT_CHECK_DOUBLE_DISPATCHER_H_INCLUDED
#define XRAY_COLLISION_CONTAINMENT_CHECK_DOUBLE_DISPATCHER_H_INCLUDED

#include <xray/collision/geometry_double_dispatcher.h>

#ifndef MASTER_GOLD

#include <xray/render/facade/debug_renderer.h>

#endif

namespace xray
{
	namespace collision
	{
		struct geometry_instance;

		struct containment_check_double_dispatcher: geometry_double_dispatcher, boost::noncopyable
		{
			containment_check_double_dispatcher(
				geometry_instance const& bounding_volume,
				geometry_instance const& testee
			);

#ifndef MASTER_GOLD
			containment_check_double_dispatcher(
				geometry_instance const& bounding_volume,
				geometry_instance const& testee,
				render::debug::renderer* renderer,
				render::scene_ptr const* scene
			);
#endif

			bool			check_contains	( );
			virtual	void	dispatch	( geometry const& bounding_volume,			geometry const& testee );
			
			virtual	void	dispatch	( box_geometry const& bounding_volume,		box_geometry const& testee );
			virtual	void	dispatch	( box_geometry const& bounding_volume,		sphere_geometry const& testee );
			virtual	void	dispatch	( box_geometry const& bounding_volume,		cylinder_geometry const& testee );

			virtual	void	dispatch	( sphere_geometry const& bounding_volume,	box_geometry const& testee );
			virtual	void	dispatch	( sphere_geometry const& bounding_volume,	sphere_geometry const& testee );
			virtual	void	dispatch	( sphere_geometry const& bounding_volume,	cylinder_geometry const& testee );

			virtual	void	dispatch	( cylinder_geometry const& bounding_volume,	box_geometry const& testee );
			virtual	void	dispatch	( cylinder_geometry const& bounding_volume,	sphere_geometry const& testee );
			virtual	void	dispatch	( cylinder_geometry const& bounding_volume,	cylinder_geometry const& testee );

		private:
			geometry_instance const&	m_bounding_volume;
			geometry_instance const&	m_testee;
			bool						m_result;

		//debug
#ifndef MASTER_GOLD
		private:
			render::debug::renderer*	m_renderer;
			render::scene_ptr const*	m_scene;
#endif

		}; // class containment_check_double_dispatcher
	} // namespace collision
} // namespace xray

#endif // #ifndef XRAY_COLLISION_CONTAINMENT_CHECK_DOUBLE_DISPATCHER_H_INCLUDED