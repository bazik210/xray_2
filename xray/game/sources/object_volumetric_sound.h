////////////////////////////////////////////////////////////////////////////
//	Created		: 13.10.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOLUMETRIC_SOUND_H_INCLUDED
#define VOLUMETRIC_SOUND_H_INCLUDED

#include "object.h"
#include <xray/sound/sound_emitter.h>

namespace xray {
namespace collision {
	class geometry_instance;
} // namespace collision
} // namespace xray

namespace stalker2 {

class object_volumetric_sound : public game_object_, public sound::sound_producer
{
	typedef game_object_				super;
public:
					object_volumetric_sound	( game_scene& w );
	virtual			~object_volumetric_sound( );
	virtual void	load					( configs::binary_config_value const& t );
	virtual void	load_contents			( );
	virtual void    load_custom				( fs::path_string m_sound_name );
	virtual void	unload_contents			( );

	virtual pcstr	get_description() const { return "volumetric sound"; };
	virtual	float3	get_source_position(float3 const& requester) const { return m_position; };
private:
			void	on_collision_object_loaded	( game_object_ptr_ const& object );
			void	on_config_loaded			( resources::queries_result& data );
			void	on_sound_loaded				( resources::queries_result& data );
public:
			void	play();
private:
	fs::path_string							m_sound_name;
	xray::sound::sound_emitter_ptr			m_emitter;
	xray::sound::sound_instance_proxy_ptr	m_proxy;
	xray::sound::sound_scene_ptr			m_sound_scene;
	xray::sound::world_user&				m_world_user;
	xray::collision::geometry_instance*		m_collision_geometry;
	float									m_radius;
	float3									m_position;
	game_scene&								m_game_scene;
}; // class volumetric_sound

} // namespace stalker2

#endif // #ifndef VOLUMETRIC_SOUND_H_INCLUDED