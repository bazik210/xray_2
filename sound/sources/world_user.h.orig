////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.02.2011
//	Author		: Dmitriy Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_WORLD_USER_H_INCLUDED
#define XRAY_WORLD_USER_H_INCLUDED

#include <xray/sound/world_user_base.h>
#include <xray/sound/api.h>
#include "two_way_threads_channel.h"
#include "sound_instance_proxy_internal.h"

namespace xray {
namespace sound {

class sound_world;

class world_user :	public world_user_base,
					private boost::noncopyable
{
public:
						world_user			( sound_world* owner, base_allocator_type* allocator );
	virtual				~world_user			( );

	virtual void		initialize			( );
	virtual void		finalize			( );

	virtual void	set_listener_properties	( sound_scene_ptr& scene, float4x4 const& inv_view_matrix );
	virtual	void	set_active_sound_scene	(	sound_scene_ptr& scene,
												u32 fade_in_time,
												u32 fade_out_old_scene_time  );

	virtual void		mute_all_sounds		( bool mute );

	virtual void		register_receiver	( sound_scene_ptr& scene, sound_receiver& receiver );
	virtual void		move_receiver		( sound_scene_ptr& scene, sound_receiver& receiver );
	virtual void		unregister_receiver	( sound_scene_ptr& scene, sound_receiver& receiver );

	virtual void		dispatch_callbacks	( );

			void		process_orders		( );
			void		process_responses	( );

			void		add_order			( sound_order* order );
			void		add_response		( sound_response* response );

			// temp
			two_way_threads_channel&	get_channel		( );
			base_allocator_type*		get_allocator	( ) const;

	inline	sound_world*				get_sound_world	( ) const { return m_owner_world; }
private:
	sound_world*					m_owner_world;
	two_way_threads_channel			m_channel;

	base_allocator_type*			m_allocator;
}; // class world_user

} // namespace sound
} // namespace xray

#endif // #ifndef XRAY_WORLD_USER_H_INCLUDED
