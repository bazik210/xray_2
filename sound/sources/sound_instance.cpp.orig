////////////////////////////////////////////////////////////////////////////
//	Created		: 14.02.2011
//	Author		: Tetyana Meleshchenko
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_instance.h"
#include <xray/sound/sound_instance_proxy.h>
#include "world_user.h"
#include "sound_world.h"
#include "functor_command.h"
#include "voice_factory.h"
#include "sound_buffer.h"

namespace xray {
namespace sound {

enum
{
	max_queued_buffers = 3,
};

bool compare_sound_instances_by_id::operator() ( sound_instance const& left, sound_instance const& right ) const
{
	return left.get_id() < right.get_id();
}

static u32 time_in_msec_to_pcm	( encoded_sound_ptr const& encoded_sound, u32 time, playback_mode mode = once )
{
	if ( mode == once )
		return	( time * (encoded_sound->get_samples_per_sec( ))) / 1000;
	else 
		return (( time % encoded_sound->get_length_in_msec( ) ) * encoded_sound->get_samples_per_sec( )) / 1000;
}


sound_instance::sound_instance	(
		sound_spl_ptr const& spl,
		encoded_sound_ptr const& encoded_sound,
		world_user& user,
		sound_instance_proxy_internal* const creator,
		float composite_sounds_offset
	) :
	m_encoded_sound						( encoded_sound ),
	m_user								( user ),
	m_playing_offset					( composite_sounds_offset ),
	m_delay_in_msec						( 0 ),
	m_stream_cursor_pcm					( 0 ),
	m_is_playing						( false ),
	m_is_positioned						( false ),
	m_is_listening						( false ),
	m_is_start_listening_sound_requested( false ),
	m_is_stop_listening_sound_requested	( false ),
	m_listening_sound_is_pausing		( false ),
	m_stop_propagating_time				( 0 ),
	m_propagating_time					( 0 ),
	m_instance_proxy					( creator ),
	m_position							( 0.0f, 0.0f, 0.0f ),
	m_La								( 0.0f ),
	m_Lp								( 0.0f ),
	m_spl								( 50.0f ),
	m_buffer_playing_offset				( 0 ),
	m_stop_propagation_distance			( 0 ),
	m_propagating_state					( not_yet_propagating ),
	m_playback_mode						( once ),
	m_propagating_request_state			( nothing_requested ),
	m_sound_producer					( 0 ),
	m_ignorable_receiver				( 0 ),
	m_volume_curve						( 0 ),
	m_sound_spl							( spl )
{
//	channels_type type	= m_encoded_sound->get_channels_type	( );
//	u32 samples_per_sec	= m_encoded_sound->get_samples_per_sec	( );
	m_length			= m_encoded_sound->get_length_in_msec	( );

//	threading::interlocked_exchange( m_instance_proxy->m_length_in_msec, m_length );

	//if ( m_user.get_sound_world()->is_audio_device_exist( ) )
	//	m_voice		= m_user.get_sound_world()->get_voice(this, type, samples_per_sec);
	//else
	//	m_voice		= 0;

	m_stop_propagation_distance	= math::pow( 10, (m_spl - 20) / 20 );
//	m_instance_proxy->increment_instances_count();
}

sound_instance::~sound_instance	( )
{
	if ( m_voice )
		m_user.get_sound_world()->free_voice( m_voice );
}

u32 sound_instance::get_id		( ) const
{
	return m_instance_proxy->get_id();
}

bool sound_instance::is_playing_once	( ) const
{
	return m_instance_proxy->is_playing_once();
}

void sound_instance::finish_playing_once		( )
{
	R_ASSERT									( is_playing_once( ) );
	stop										( true );

	m_is_playing								= false;

//	m_instance_proxy->decrement_instances_count	( );

	m_instance_proxy->set_callback_pending		( true );
	m_user.add_response			( XRAY_NEW_IMPL(
										m_user.get_channel().responses.owner_allocator(),
										functor_response )
									( boost::bind( &sound_instance::execute_user_callback, this ) ) );
	LOG_DEBUG					( "sound_instance::execute_user_callback added to resonses" );
}

void sound_instance::play		(	playback_mode mode,
									bool is_positioned,
									float3 const& position,
									sound_producer const* const producer,
									sound_receiver const* const ignorable_receiver
								)
{
	LOG_DEBUG				( "sound_instance::play" );
	CURE_ASSERT				( m_propagating_state == not_yet_propagating, { LOG_ERROR( "attempt to play sound which is already playing" ); return ; });
	m_playback_mode			= mode;
	
	m_is_positioned			= is_positioned;
	m_is_playing			= true;

	m_sound_producer		= producer;
	m_ignorable_receiver	= ignorable_receiver;
	start_propagating		( );

	if ( is_positioned )
	{
		R_ASSERT				( m_encoded_sound->get_channels_type( ) == mono );
		m_position				= position;
		float distance_offset	= m_user.get_sound_world( )->get_distance_to_listener( m_position );
		m_delay_in_msec			= static_cast<u32>( distance_offset / m_user.get_sound_world()->get_speed_of_sound() );
	}
	else
	{
		R_ASSERT				( m_encoded_sound->get_channels_type( ) != mono );
		m_delay_in_msec			= 0;
	}

	if ( m_voice )
		m_is_start_listening_sound_requested	= true;
}

void sound_instance::stop			( bool immediately_stop_propagation )
{
	LOG_DEBUG						( "sound_instance::stop" );

	m_stop_propagating_time			= 0;

	if ( m_propagating_state == propagating )
	{
		m_stop_propagating_time		= immediately_stop_propagation ? 0 : m_propagating_time + m_delay_in_msec;
	}

	m_propagating_request_state		= stop_requested;

}

void sound_instance::pause			( )
{
	LOG_DEBUG							( "sound_instance::pause" );
	CURE_ASSERT							( m_propagating_state == propagating, { LOG_ERROR( "attempt to pause sound which is not propagating" ); return ; } );

//	threading::interlocked_exchange		( m_instance_proxy->m_is_propagating_paused, 1 );

	if ( m_voice )	
		m_listening_sound_is_pausing		= true;

	pause_propagating					( );
}

void sound_instance::resume			( )
{
	LOG_DEBUG							( "sound_instance::resume" );
	CURE_ASSERT							( m_propagating_state == propagating_paused, { LOG_ERROR( "attempt to resume sound which is not paused" ); return ; } );

//	threading::interlocked_decrement	( m_instance_proxy->m_is_propagating_paused );
	resume_propagating					( );
	m_propagating_request_state			= resume_requested;

}

void sound_instance::seek			( u32 time_in_msec )
{
	LOG_DEBUG							( "sound_instance::seek" );
	CURE_ASSERT							( !m_is_positioned, { LOG_ERROR( "attempt to seek positional sound" ); return ; } );

	seek_propagating					( time_in_msec );
}

void sound_instance::refill_buffers	( )
{
	if ( !m_voice )
		return;

	u32 buffers_queued				= m_voice->buffers_queued( );
	u64 pcm_total					= m_encoded_sound->get_length_in_pcm();

	while ( ( buffers_queued < max_queued_buffers ) && ( m_stream_cursor_pcm < pcm_total ) )
	{
		sound_buffer* buffer = m_user.get_sound_world()->get_sound_buffer( m_encoded_sound, m_stream_cursor_pcm, m_stream_cursor_pcm);
		m_voice->submit_source_buffer	( buffer, m_buffer_playing_offset );
		if ( m_buffer_playing_offset )
			 m_buffer_playing_offset = 0;
		++buffers_queued;
	}
}

void sound_instance::on_finish_playing		( )
{
	LOG_DEBUG							( "sound_instance::on_finish_playing" );
	m_is_playing						= false;

//	m_instance_proxy->decrement_instances_count();

	if ( !m_user.get_sound_world()->is_destroying() && m_playback_mode != looped  ) 
	{
		m_instance_proxy->set_callback_pending		( true );
		m_user.add_response			( XRAY_NEW_IMPL(
										m_user.get_channel().responses.owner_allocator(),
										functor_response )
									( boost::bind( &sound_instance::execute_user_callback, this ) ) );
	}
}

void sound_instance::on_finish_listening	( )
{
	LOG_DEBUG							( "sound_instance::on_finish_listening" );
	if ( m_voice )
	{
		m_stream_cursor_pcm					= 0;
		m_is_listening						= false;
		m_is_stop_listening_sound_requested	= false;
//		threading::interlocked_exchange		( m_instance_proxy->m_is_listening, 0 );
		m_voice->stop						( );
	}
}

command_result_enum sound_instance::execute_user_callback	( ) const
{
	R_ASSERT							( m_instance_proxy );
	if ( !m_instance_proxy->is_destruction_pending() )
	{
		m_instance_proxy->execute_callback			( );
	}
	m_instance_proxy->set_callback_pending		( false );
	return command_result_executed;
}

void sound_instance::tick( u32 time_delta )
{
	if ( m_propagating_state == propagating )
	{
		m_propagating_time		+= time_delta;

//		threading::interlocked_exchange( m_instance_proxy->m_propagated_time, m_propagating_time );
//		if ( m_propagating_time > m_length )
//			threading::interlocked_exchange( m_instance_proxy->m_propagated_end_time, static_cast<u32>( m_propagating_time - m_length ) );


		if ( !m_is_listening && m_voice )
		{
			if ( m_propagating_time > m_delay_in_msec && m_is_start_listening_sound_requested )
				play_listening_sound	( );
		}

		if ( is_time_to_stop_propagating ( ) )
			stop_propagating		( );


		if ( m_propagating_request_state == stop_requested && m_propagating_time > m_stop_propagating_time )
		{
			stop_propagating		( );
		}

		if ( m_is_playing && m_propagating_time > m_length )
			on_finish_playing		( );
	}
	else
	{
		if ( m_propagating_request_state == stop_requested )
			stop_propagating		( );
	}
}

void sound_instance::play_listening_sound ( )
{
	LOG_DEBUG							( "sound_instance::play_listening_sound" );

	if ( m_user.get_sound_world()->is_audio_device_exist( ) )
		m_user.get_sound_world( )->calculate_3d_sound( this );

	if ( m_voice )
	{
		m_is_start_listening_sound_requested = false;

		m_is_listening					= true;
		m_stream_cursor_pcm				= 0;
		refill_buffers					( );
		m_voice->start					( );

//		threading::interlocked_exchange	( m_instance_proxy->m_is_listening, 1 );
	}
}

void sound_instance::stop_listening_sound ( )
{
	LOG_DEBUG			( "sound_instance::stop_listening_sound" );

	if ( m_voice )
	{
		m_is_stop_listening_sound_requested	= true;

		m_voice->stop						( );
		m_voice->flush_source_buffers		( );
	}

	m_buffer_playing_offset				= 0;
}

void sound_instance::pause_listening_sound	( )
{
	LOG_DEBUG		( "sound_instance::pause_listening_sound" );
	if ( m_is_listening )
	{
		if ( m_voice )
		{
			m_is_listening					= false;
			m_voice->stop					( );
			m_voice->flush_source_buffers	( );
		}
	}
	else
	{
		if ( m_voice )
			m_listening_sound_is_pausing	= false;
	}
}

void sound_instance::resume_listening_sound	( )
{

	LOG_DEBUG		( "sound_instance::resume_listening_sound" );
	m_stream_cursor_pcm				= find_nearest_adjective_pcm_offset ( );
	
	if ( m_playback_mode == once )
		m_buffer_playing_offset		= time_in_msec_to_pcm( m_encoded_sound, m_propagating_time - m_delay_in_msec ) - find_nearest_adjective_pcm_offset( );
	else
		m_buffer_playing_offset		= time_in_msec_to_pcm( m_encoded_sound, m_propagating_time - m_delay_in_msec, looped ) - find_nearest_adjective_pcm_offset( );
	
	if ( m_user.get_sound_world()->is_audio_device_exist( ) )
		m_user.get_sound_world( )->calculate_3d_sound( this );

	if ( m_voice )
	{
		m_is_listening					= true;
		refill_buffers					( );
		m_voice->start					( );
//		threading::interlocked_exchange	( m_instance_proxy->m_is_listening, 1 );
	}
}


void sound_instance::start_propagating	( u32 offset_in_msec )
{
	LOG_DEBUG		( "sound_instance::start_propagating" );

	m_propagating_request_state			= nothing_requested;

	m_propagating_time					= offset_in_msec;
	m_propagating_state					= propagating;
//	threading::interlocked_increment	( m_instance_proxy->m_is_propagating );

	if ( m_voice )
	{
		m_is_listening						= false;
//		threading::interlocked_exchange		( m_instance_proxy->m_is_propagating_paused, 0 );
	}
}

void sound_instance::stop_propagating	( )
{
	LOG_DEBUG		( "sound_instance::stop_propagating" );

//	if ( m_propagating_state != propagating )
//	threading::interlocked_decrement	( m_instance_proxy->m_is_propagating );

	m_propagating_request_state			= nothing_requested;
	m_propagating_state					= not_yet_propagating;
	m_propagating_time					= 0;
//	threading::interlocked_exchange		( m_instance_proxy->m_is_propagating_paused, 0 );

	if ( m_is_listening && m_voice )
		stop_listening_sound			( );
}

void sound_instance::pause_propagating	( )
{
//	threading::interlocked_increment	( m_instance_proxy->m_is_propagating_paused );

	LOG_DEBUG		( "sound_instance::pause_propagating" );

	m_propagating_state					= propagating_paused;

	if ( m_is_listening && m_voice )
	{
		pause_listening_sound			( );
	}
	else
	{
		if ( m_voice )
			m_listening_sound_is_pausing	= false;
	}
}

void sound_instance::resume_propagating	( )
{

	LOG_DEBUG		( "sound_instance::resume_propagating" );
	m_propagating_request_state	= nothing_requested;
	
	m_propagating_state			= propagating;
	if ( !m_is_listening && m_voice )
	{
		if ( m_propagating_time < m_delay_in_msec )
			return;
		else if ( m_playback_mode == looped )
			resume_listening_sound		( );
		else if ( m_propagating_time < m_delay_in_msec + m_length )
			resume_listening_sound		( );
	}
}

void sound_instance::seek_propagating	( u32 time_in_msec )
{
	LOG_DEBUG		( "sound_instance::seek_propagating" );

	propagating_state old_state			= m_propagating_state;
	pause_propagating					( );
	m_propagating_time					= time_in_msec;
	if ( old_state == propagating )
		resume_propagating				( );
}

u32 sound_instance::find_nearest_adjective_pcm_offset	( ) const
{
	u32 pcm_cursor					= 0;

	if ( m_propagating_state == once )
	{
		pcm_cursor = time_in_msec_to_pcm( m_encoded_sound, m_propagating_time - m_delay_in_msec );
	}
	else
	{
		pcm_cursor = time_in_msec_to_pcm( m_encoded_sound, m_propagating_time - m_delay_in_msec, looped );
	}
	return ( pcm_cursor / ( sound_buffer_size / m_encoded_sound->get_bytes_per_sample( ))) * ( sound_buffer_size / m_encoded_sound->get_bytes_per_sample( ) );
}

bool sound_instance::is_time_to_stop_propagating		( ) const
{
	if ( m_playback_mode == looped )
		return false;

	if ( m_propagating_time < m_length )
		return false;

	if ( m_is_listening && m_voice )
		return false;

	float sound_propagating_end_position	= ( m_propagating_time - m_length ) * m_user.get_sound_world()->get_speed_of_sound() ;
	LOG_DEBUG								( "sound_propagating_end_position = %f, stop_propagation_distance = %f", sound_propagating_end_position, m_stop_propagation_distance );
	return sound_propagating_end_position > m_stop_propagation_distance;
}

float sound_instance::get_propagating_distance( ) const
{ 
	R_ASSERT( m_is_positioned );
	return m_propagating_time * m_user.get_sound_world( )->get_speed_of_sound();
}

float sound_instance::get_end_propagating_distance( ) const			
{	
	R_ASSERT( m_is_positioned );
	if ( m_propagating_time < m_length )
		return 0.0f;
	else
		return ( m_propagating_time - m_length ) * m_user.get_sound_world( )->get_speed_of_sound(); 
}


} // namespace sound
} // namespace xray
