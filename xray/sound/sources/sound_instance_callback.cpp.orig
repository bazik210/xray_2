////////////////////////////////////////////////////////////////////////////
//	Created		: 03.03.2011
//	Author		: Dmitry Kulikov
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_instance.h"
#include "two_way_threads_channel.h"
#include "world_user.h"
#include "sound_world.h"
#include "functor_command.h"
#include "voice_factory.h"

namespace xray {
namespace sound {

static bool _logging_initialized = false;

static void check_logging()
{
	if(!_logging_initialized)
	{
//		xray::core::debug::on_thread_spawn		( );
		xray::logging::on_thread_spawn			( );

		_logging_initialized = true;
	}
}

// Called just before this voice's processing pass begins.
void sound_instance::on_voice_processing_pass_start	( u32 bytes_required )
{
	//check_logging();
	XRAY_UNREFERENCED_PARAMETERS( bytes_required );
	//LOG_DEBUG("OnVoiceProcessingPassStart: %d", bytes_required);
}

// Called just after this voice's processing pass ends.
void sound_instance::on_voice_processing_pass_end	( )
{
	//check_logging();
	//LOG_DEBUG("OnVoiceProcessingPassEnd");
}

// Called when this voice has just finished playing a buffer stream
void sound_instance::on_stream_end					( )
{
	check_logging();
	
	m_user.get_sound_world()->add_xaudio_order(
		MT_NEW ( functor_order ) (
			boost::bind( &sound_instance::on_stream_end_impl, this )
		)
	);
	LOG_DEBUG( "on_stream_end xaudio order with id: %d", m_instance_proxy->get_id() );
}

// Called when this voice is about to start processing a new buffer.
void sound_instance::on_buffer_start					( pvoid buffer_context )
{
	XRAY_UNREFERENCED_PARAMETERS( buffer_context );
	//check_logging();
	//LOG_DEBUG("OnBufferStart: %x", buffer_context);
}

// Called when this voice has just finished processing a buffer.
// The buffer can now be reused or destroyed.
void sound_instance::on_buffer_end					( pvoid buffer_context )
{
	check_logging();

	
//	threading::yield( 100 );

	m_user.get_sound_world()->add_xaudio_order(
				MT_NEW ( functor_order ) (
				boost::bind( &sound_instance::on_buffer_end_impl, this, buffer_context )
			)
		);

	//LOG_DEBUG( "on_buffer_end xaudio order with id: %d", m_instance_proxy->get_id() );
}

// Called when this voice has just reached the end position of a loop.
void sound_instance::on_loop_end					( pvoid buffer_context )
{
	XRAY_UNREFERENCED_PARAMETERS( buffer_context );
	check_logging();
	//LOG_DEBUG("OnLoopEnd: %x", buffer_context);
}

// Called in the event of a critical error during voice processing,
void sound_instance::on_voice_error					( pvoid buffer_context, HRESULT error )
{
	check_logging();
	m_user.get_sound_world()->add_xaudio_order(
		MT_NEW ( functor_order ) (
			boost::bind( &sound_instance::on_buffer_error_impl, this, buffer_context )
		)
	);
	LOG_ERROR("OnVoiceError: %d, %x", error, buffer_context);
	LOG_ERROR( "on_voice_error xaudio order with id: %d", m_instance_proxy->get_id() );
}

command_result_enum sound_instance::on_stream_end_impl( )
{
	if ( m_voice->buffers_queued( ) == 0 && !m_is_stop_listening_sound_requested && m_propagating_state != propagating_paused && m_playback_mode == once )
	{
		on_finish_listening			( );
	}

	if ( m_playback_mode == looped )
	{
		if ( !m_is_stop_listening_sound_requested )
			play_listening_sound		( );
		else if ( m_voice->buffers_queued( ) == 0 && m_propagating_state != propagating_paused )
					on_finish_listening			( );
	}

	//LOG_DEBUG ( "on_stream_end_impl id %d", m_instance_proxy->get_id( ) );  
	return					command_result_executed;
}

command_result_enum sound_instance::on_buffer_end_impl( void* pcontext )
{
	//LOG_DEBUG ( "on_buffer_end_impl id %d", m_instance_proxy->get_id( ) );  

	sound_buffer* sb		= static_cast_checked<sound_buffer*>(pcontext);
	m_user.get_sound_world()->free_sound_buffer ( sb );

	if ( m_is_stop_listening_sound_requested )
	{
		if ( m_voice->buffers_queued( ) == 0 )
		{
			on_finish_listening			( );
		}
	}

	if ( m_listening_sound_is_pausing )
	{
		if ( m_voice->buffers_queued( ) == 0 )
		{
			m_listening_sound_is_pausing	= false;
			m_is_listening					= false;
		}
	}

	if( m_is_listening && !m_is_stop_listening_sound_requested && m_propagating_state != propagating_paused )
		refill_buffers		( );

	return command_result_executed;
}

command_result_enum sound_instance::on_buffer_error_impl( void* pcontext)
{
	XRAY_UNREFERENCED_PARAMETERS( pcontext );
	LOG_DEBUG("---voice on_buffer_error");
	return command_result_executed;
}

} // namespace sound
} // namespace xray

