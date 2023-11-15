////////////////////////////////////////////////////////////////////////////
//	Created		: 04.05.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef CAMERA_DIRECTOR_H_INCLUDED
#define CAMERA_DIRECTOR_H_INCLUDED

#include "object.h"
#include "object_behaviour.h"

namespace stalker2{

class game_camera;
class game_world;

class camera_director : public game_object_, 
						public object_controlled
{
	typedef game_object_ super;
public:
						camera_director				( game_scene& w );
	void				switch_to_camera			( game_camera* c, pcstr camera_name );
	game_camera*		get_active_camera			( ) const				{ return m_active_camera; }
	void				tick						( );
	void				apply						( );
	float4x4 const&		get_inverted_view_matrix	( ) const				{ return m_inverted_view; }
	float4x4 const&		get_projection_matrix		( ) const				{ return m_projection; }
	void				set_position_direction		( math::float3 const& p, math::float3 const& d );
	virtual void		on_focus					( bool b_focus_enter);

private:
	float4x4			m_inverted_view;
	float4x4			m_projection;

	game_camera*		m_active_camera;
	game_world*			m_game_world;
	float				m_aspect;
}; // class camera_director

typedef	intrusive_ptr<
			camera_director,
			resources::unmanaged_intrusive_base,
			threading::single_threading_policy
		> camera_director_ptr;

} // namespace stalker2

#endif // #ifndef CAMERA_DIRECTOR_H_INCLUDED