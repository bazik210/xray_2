////////////////////////////////////////////////////////////////////////////
//	Created		: 01.02.2011
//	Author		: Dmitriy Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_SOUND_H_INCLUDED
#define OBJECT_SOUND_H_INCLUDED

#include "object.h"
#include <xray/sound/sound.h>
#include <xray/sound/sound_emitter.h>
#include <xray/sound/world.h>

namespace xray {
	namespace collision {
		class geometry_instance;
	} // namespace collision
} // namespace xray

namespace stalker2 {

class object_sound : public game_object_
{
	typedef game_object_	super;
public:
							object_sound	( game_scene& w );
	virtual					~object_sound	( );
	virtual void			load(configs::binary_config_value const& t);
	virtual void			load_contents	( );
	virtual	void			set_transform	( float4x4 const& transform );
	virtual void			unload_contents	( );


private:
			void			initialize_collision( );
			void			on_sound_loaded	( resources::queries_result& data );
			void			load_sound		( );

			double			min_seek_val		( ) { return 0.0f; }
			double			max_seek_val		( );

			void			emit				( );

	xray::sound::sound_emitter_ptr					m_sound_emitter_ptr;
	xray::sound::sound_scene_ptr					m_sound_scene;
	xray::sound::world_user&					    m_world_user;
	xray::sound::sound_instance_proxy_ptr			m_proxy;
	fs::path_string m_wav_file_name;
	float3			m_position;
	float3			m_direction;
	float3			m_up;

	float			m_seek_pos;
	int				m_sound_type;
	int				m_emitter_type;

	bool			m_loaded;
	bool			m_selected;
	bool			m_positional;

	game_scene&		m_game_scene;
	xray::collision::geometry_instance* m_collision;
	fs::path_string		m_sound_name;
	math::float4x4		m_transform;
public:

}; // class object_sound

} // namespace stalker2

#endif // #ifndef OBJECT_SOUND_H_INCLUDED
