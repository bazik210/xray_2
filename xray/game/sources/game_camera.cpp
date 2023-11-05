////////////////////////////////////////////////////////////////////////////
//	Created		: 28.04.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_camera.h"

namespace stalker2{

game_camera::game_camera( game_scene& w )
:super			( w ),
m_game_scene	( w ),
m_near_plane	( 0.1f ),
m_far_plane		( 5000.0f )
{
}

float game_camera::m_vertical_fov = 58.5f; //70.5f

void game_camera::set_position_direction( math::float3 const& p, math::float3 const& d )
{
	m_inverted_view_matrix	= math::invert4x3( math::create_camera_direction( p, d, float3(0.f, 1.f, 0.f ) ) );
}

void game_camera::on_activate( camera_director* cd )
{
	m_inverted_view_matrix = cd->get_inverted_view_matrix( );
}

math::float4x4  game_camera::get_projection_matrix( float2 const& window_size, float pov ) const			
{ 
//	float2 size			= m_game_scene.get_game().engine().get_render_window_size();

	if(pov) m_vertical_fov = pov;

	float const aspect	= window_size.x / window_size.y; //width / height

	return math::create_perspective_projection( math::deg2rad(pov),
												aspect, 
												m_near_plane, 
												m_far_plane );
}

}