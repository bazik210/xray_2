////////////////////////////////////////////////////////////////////////////
//	Created		: 14.02.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef SOUND_INSTANCE_H_INCLUDED
#define SOUND_INSTANCE_H_INCLUDED

#include "voice_callback_handler.h"
#include <xray/sound/sound.h>
#include <xray/sound/sound_instance_proxy.h>
#include <xray/sound/sound_spl.h>

namespace xray {
namespace sound {

class world_user;
class voice_bridge;
class sound_receiver;
class sound_instance_proxy_internal;
class encoded_sound_interface;

typedef	resources::resource_ptr < encoded_sound_interface, resources::unmanaged_intrusive_base > encoded_sound_ptr;

enum sound_instance_state
{
	not_yet_played		= 0,
	playing				= 1,
	paused				= 2,
};

enum propagating_state
{
	not_yet_propagating		= 0,
	propagating				= 1,
	propagating_paused		= 2,
//	propagating_finished	= 4,
};

enum propagating_request_state
{
	nothing_requested		= 0,
	pause_requested			= 1,
	resume_requested		= 2,
	stop_requested			= 3,
};

class sound_instance : private boost::noncopyable
{
public:
		typedef boost::function< command_result_enum ( ) >	callback_type;
		typedef boost::function< void ( ) >					destruction_callback_type;

								sound_instance		(
									sound_spl_ptr const&					spl,
									encoded_sound_ptr const&				encoded_sound,
									world_user&								user,
									sound_instance_proxy_internal* const	creator,
									float									composite_sounds_offset	= .0f
								);
	virtual						~sound_instance				( );

			void					play					(
													playback_mode mode		= once,
													bool is_positioned		= false,
													float3 const& position	= float3( .0f, .0f, .0f ),
													sound_producer const* const producer = 0,
													sound_receiver const* const ignorable_receiver = 0
															);
			void					stop					( bool immediately_stop_propagation = false );
			void					pause					( );
			void					resume					( );
			void					seek					( u32 time_in_msec );
			void					tick					( u32 time_delta );
	inline	bool					is_playing				( ) const			{ return m_is_listening; }
	inline	bool					is_paused				( ) const			{ return m_propagating_state == propagating_paused; }
	inline	bool					is_hearable				( ) const			{ return m_is_listening; }
			u32						get_id					( ) const;

			bool					is_playing_once			( ) const;
			void					finish_playing_once		( );

	inline	bool					is_positioned			( ) const			{ return m_is_positioned; }

	inline  void					set_position			( float3 const& p )	{ R_ASSERT( m_is_positioned ); m_position = p; }
	inline	float3 const&			get_position			( ) const			{ R_ASSERT( m_is_positioned ); return m_position; }
	
			float					get_propagating_distance( ) const;			
			float				get_end_propagating_distance( ) const;
	inline	float				get_max_propagating_distance( ) const			{ return m_stop_propagation_distance; }
	inline	propagating_state		get_propagating_state	( ) const			{ return m_propagating_state; } 

	inline	float					get_rms					( ) const			{ return 0; /*m_propagated_time <= m_length ? m_sound->get_rms( m_propagated_time ) : 0.0f;*/ }
	inline	sound_spl_ptr const&	get_sound_spl			( ) const			{ return m_sound_spl; }
	inline	voice_bridge*			get_voice				( )					{ return m_voice; }
	
	inline	bool					can_apply_command		( ) const			{ return !m_listening_sound_is_pausing && !m_is_stop_listening_sound_requested && m_propagating_request_state != stop_requested; }

	inline	bool					has_producer			( ) const			{ return m_sound_producer != 0; }
	inline	sound_producer const*	get_producer			( ) const			{ return m_sound_producer; }
	inline	sound_receiver const*	get_ignorable_receiver	( ) const			{ return m_ignorable_receiver; }

	inline	world_user&				get_world_user			( )	const			{ return m_user; }

// voice_callback_handler stuff
// must be called from voice_bridge class only
public:
			void		on_voice_processing_pass_start		( u32 bytes_required );
			void		on_voice_processing_pass_end		( );
			void		on_stream_end						( );
			void		on_buffer_start						( pvoid buffer_context );
			void		on_buffer_end						( pvoid buffer_context );
			void		on_loop_end							( pvoid buffer_context );
			void		on_voice_error						( pvoid buffer_context, HRESULT error );
	

// in sound thread callback implementation
private:
	command_result_enum	on_stream_end_impl					( );
	command_result_enum	on_buffer_end_impl					( pvoid pcontext );
	command_result_enum	on_buffer_error_impl				( pvoid pcontext );

private:
			void		refill_buffers						( );
			void		on_finish_playing					( );
			void		on_finish_listening					( );
command_result_enum		execute_user_callback				( ) const;

			void		play_listening_sound				( );
			void		stop_listening_sound				( );
			void		pause_listening_sound				( );
			void		resume_listening_sound				( );

			void		start_propagating					( u32 offset_in_msec = 0 );
			void		stop_propagating					( );
			void		pause_propagating					( );
			void		resume_propagating					( );
			void		seek_propagating					( u32 time_in_msec );

			bool		is_time_to_stop_propagating			( ) const;
			u32			find_nearest_adjective_pcm_offset	( ) const;


public:
	typedef boost::intrusive::set_member_hook< boost::intrusive::link_mode< boost::intrusive::auto_unlink > > sound_instance_member_hook;
	sound_instance_member_hook			m_hook;
private:
	float3									m_position;
	u64										m_length;
	encoded_sound_ptr						m_encoded_sound;
	sound_instance_proxy_internal* const	m_instance_proxy;
	voice_bridge*							m_voice;
	sound_producer const*					m_sound_producer;
	sound_receiver const*					m_ignorable_receiver;
	world_user&								m_user;
	sound_spl_ptr const&					m_sound_spl;

	X3DAUDIO_DISTANCE_CURVE*				m_volume_curve;

	float									m_playing_offset;
	float									m_spl;
	float									m_stop_propagation_distance;

	u32										m_stream_cursor_pcm;
	u32										m_buffer_playing_offset;

	u32										m_propagating_time;
	u32										m_delay_in_msec;
	u32										m_stop_propagating_time;
	u32										m_pause_propagating_time;

	bool									m_is_playing;
	bool									m_is_positioned;
	bool									m_is_sound_world_destroying;

	bool									m_is_propagating;
	bool									m_is_listening;
	bool									m_is_start_listening_sound_requested;
	bool									m_is_stop_listening_sound_requested;
	bool									m_listening_sound_is_pausing;

	propagating_state						m_propagating_state;
	propagating_request_state				m_propagating_request_state;
	playback_mode							m_playback_mode;
	// for hdr audio test
public:
	float					m_La;
	float					m_Lp;
}; // class sound_instance

struct compare_sound_instances_by_id
{
	bool operator () ( sound_instance const& left, sound_instance const& right ) const;
}; // struct compare_sound_instances_by_id

struct sound_instances_comparator
{
	inline bool operator()	(  u32 instance_id, sound_instance const& instance ) const;
	inline bool operator()	(  sound_instance const& instance, u32 instance_id ) const;
};

typedef sound_instance* sound_instance_ptr;

typedef boost::intrusive::member_hook < sound_instance, 
										sound_instance::sound_instance_member_hook,
										&sound_instance::m_hook 
									  > sound_instance_helper_option;

typedef boost::intrusive::multiset< sound_instance,
									sound_instance_helper_option,
									boost::intrusive::compare< compare_sound_instances_by_id >,
						  	 		boost::intrusive::constant_time_size< false >
								  >	sound_instances_type;



inline bool sound_instances_comparator::operator()	(  u32 instance_id, sound_instance const& instance ) const
{
	return				instance_id < instance.get_id();
}

inline bool sound_instances_comparator::operator()	(  sound_instance const& instance, u32 instance_id ) const
{
	return				instance.get_id() < instance_id;
}


} // namespace sound
} // namespace xray

#endif // #ifndef SOUND_INSTANCE_H_INCLUDED
