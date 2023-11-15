////////////////////////////////////////////////////////////////////////////
//	Created		: 24.03.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_SCENE_H_INCLUDED
#define GAME_SCENE_H_INCLUDED

#include <xray/render/engine/base_classes.h>
#include <xray/render/facade/game_renderer.h>
#include <xray/sound/sound.h>
#include <xray/physics/engine.h>
#include "camera_director.h"


namespace xray{
	namespace physics{struct world;}
};



namespace stalker2 {
class game;

class game_scene : private xray::physics::engine,  private boost::noncopyable {
public:
							explicit		game_scene				( game& g );
							void			init_physics			( );
							virtual			~game_scene				( ) { }
							virtual void	on_activate				( );
							virtual void	on_deactivate			( );
							virtual void	tick					( );
									bool	is_active				( ) const { return m_is_active;}
									void	apply_camera			( camera_director_ptr cd );

	game&									get_game				( ) const { return m_game; }
	game_world&								get_game_world			( ) const { return *m_game_world; }
									void	set_game_world			( game_world* w ) { m_game_world = w; }
	xray::render::game::renderer&			renderer				( );
	xray::physics::world*					get_physics_world		( ) { return m_physics_world;}


	xray::render::scene_ptr const&			get_render_scene		( )	const { return m_scene; }
	xray::render::scene_view_ptr const&		get_render_scene_view	( )	const { return m_scene_view; }

	sound::sound_scene_ptr&					get_sound_scene			( ) { return m_sound_scene; }

#ifndef MASTER_GOLD
							pcstr			dbg_name				( ) const { return m_dbg_name.c_str(); }
#endif //#ifndef MASTER_GOLD

protected:
	bool									m_is_active;
	xray::render::scene_ptr					m_scene;
	xray::render::scene_view_ptr			m_scene_view;

#ifndef MASTER_GOLD
	fixed_string<128>						m_dbg_name;
#endif //#ifndef MASTER_GOLD

protected:
	game&									m_game;
	game_world*								m_game_world;
	xray::sound::sound_scene_ptr			m_sound_scene;
	float4x4								m_inverted_view_matrix;
	float4x4								m_projection_matrix;

private:
	xray::physics::world*					m_physics_world;

}; // class game_scene

} // namespace stalker2

#endif // #ifndef GAME_SCENE_H_INCLUDED