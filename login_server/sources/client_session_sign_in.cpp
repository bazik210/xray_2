////////////////////////////////////////////////////////////////////////////
//	Created		: 08.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "client_session.h"
#include "login_server.h"
#include <boost/date_time/posix_time/time_parsers.hpp>
#include <boost/date_time/posix_time/time_formatters.hpp>

#ifdef _DEBUG
extern u32 s_requests_count;
#endif // #ifdef _DEBUG

namespace xray {
	extern u32	sign_in_tolerant_attempts_count;
	extern u32	sign_in_tolerant_wait_time;
	extern u32	sign_in_intolerant_wait_time;
} // namespace xray

using xray::client_session;

void client_session::on_lobby_info_sent		(
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	LOG								( "lobby info has been sent!\r\n" );
	(void)error_code;
	(void)bytes_transferred;

	disconnect_and_destroy_this		( );
}

void client_session::send_lobby_info( u32 const session_id )
{
	LOG								( "sending lobby info...\r\n" );
	char const host[]				= "localhost";
	u32 const buffer_size			= 1 + 1 + (sizeof(host) - 1) + sizeof(session_id);
	ASSERT							( buffer_size <= sizeof(m_data) );
	pbyte buffer					= m_data;

	*buffer++						= lobby_info_message_type;

	*buffer++						= sizeof(host) - 1;

	memcpy							( buffer, host, *(buffer - 1) );
	buffer							+= *(buffer - 1);

	memcpy							( buffer, &session_id, sizeof(session_id) );
	buffer							+= sizeof(session_id);

	boost::asio::async_write		(
		m_ssl_stream,
		boost::asio::buffer( m_data, buffer - m_data ),
		make_custom_alloc_handler(
			m_allocator,
			boost::bind (
				&client_session::on_lobby_info_sent,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		)
	);
}

void client_session::add_online_user( u32 account_id, boost::posix_time::ptime const& current_time )
{
	LOG								( "adding online user\r\n" );
	update_sign_in_stats			( account_id, 0, current_time );
	remove_online_user				( account_id );
	u32 const session_id			= insert_new_online_user( account_id );
	send_lobby_info					( session_id );
}

void client_session::on_sign_in_password_received	(
		u32 const account_id,
		u32 const invalid_sign_in_attempts_count,
		u32 const seconds_to_wait,
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	if ( error_code ) {
		LOG_ERROR					( "SIGN_IN(on password received)error during reading from socket : %s\r\n", error_code.message().c_str() );
		disconnect_and_destroy_this	( );
		return;
	}

	if ( !bytes_transferred ) {
		LOG_ERROR					( "SIGN_IN(on password received): unable to read from socket\r\n" );
		disconnect_and_destroy_this	( );
		return;
	}

	LOG								( "%d bytes have been read\r\n", bytes_transferred );
#ifdef _DEBUG
	LOG								( "BEFORE on_sign_in_password_received: %d\r\n", s_requests_count );
#endif // #ifdef _DEBUG
	pbyte buffer					= &m_data[0];

	u8 const password_length		= *buffer++;
	char password[ max_password_length ];
	memcpy							( password, buffer, password_length );
	password[ password_length ]		= 0;
	buffer							+= password_length;

	digest_type test_password_digest;
	query_processor::fill_string_digest( password, test_password_digest );

	boost::posix_time::ptime const& now	= boost::posix_time::microsec_clock::local_time( );
	if ( memcmp( test_password_digest, m_password_digest, sizeof(test_password_digest) ) != 0 ) {
		LOG							( "invalid password!\r\n" );
		update_sign_in_stats		( account_id, invalid_sign_in_attempts_count, now + boost::posix_time::seconds( seconds_to_wait ) );
		send_message				( invalid_user_name_or_password_message_type, secure, read_after_message_sent );
	}
	else
		add_online_user				( account_id, now );

#ifdef _DEBUG
	LOG								( "AFTER on_sign_in_password_received: %d\r\n", s_requests_count );
#endif // #ifdef _DEBUG
}

void client_session::sign_in_on_handshaked	(
		u32 const account_id,
		u32 const invalid_sign_in_attempts_count,
		u32 const seconds_to_wait
	)
{
#ifdef _DEBUG
	LOG								( "BEFORE sign_in_on_handshaked: %d\r\n", s_requests_count );
#endif // #ifdef _DEBUG
	LOG								( "handshaked!\r\n" );
	LOG								( "waiting for password...\r\n" );
	m_handshaked					= true;
	m_ssl_stream.async_read_some	(
		boost::asio::buffer( m_data, sizeof(m_data) ),
		make_custom_alloc_handler(
			m_allocator,
			boost::bind(
				&client_session::on_sign_in_password_received,
				this,
				account_id,
				invalid_sign_in_attempts_count,
				seconds_to_wait,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		)
	);
#ifdef _DEBUG
	LOG								( "AFTER sign_in_on_handshaked: %d\r\n", s_requests_count );
#endif // #ifdef _DEBUG
}

extern long g_sign_in_count;

void client_session::process_sign_in( )
{
	BOOST_INTERLOCKED_INCREMENT		( &g_sign_in_count );
	
	pbyte buffer					= &m_data[1];
	u8 const account_name_length	= *buffer++;
	char							account_name[max_account_name_length];
	memcpy							( account_name, buffer, account_name_length );
	account_name[account_name_length] = 0;

	u32 account_id					= u32(-1);
	u32 invalid_attempts_count		= u32(-1);
	boost::posix_time::ptime		next_attempt_time;
	
	if ( m_login_server.get_queries_processor().process_sign_in( account_name, account_name_length, account_id, invalid_attempts_count, next_attempt_time, m_password_digest ) )
	{
		ASSERT						( account_id != u32(-1) );
		ASSERT						( invalid_attempts_count != u32(-1) );

		boost::posix_time::ptime const& now	= boost::posix_time::microsec_clock::local_time();
		u32 const seconds_to_wait	= invalid_attempts_count < sign_in_tolerant_attempts_count ? sign_in_tolerant_wait_time : sign_in_intolerant_wait_time;
		if ( now < next_attempt_time )
		{
			LOG_ERROR				( "SIGN_IN: sign in attempt interval violation: %s => %s\r\n", boost::posix_time::to_simple_string( next_attempt_time ).c_str(), boost::posix_time::to_simple_string( next_attempt_time + boost::posix_time::seconds( seconds_to_wait ) ).c_str() );
			update_sign_in_stats	( account_id, invalid_attempts_count, next_attempt_time + boost::posix_time::seconds( seconds_to_wait ) );
			disconnect_and_destroy_this( );
		}
		else
		{
			LOG						( "handshaking...\r\n" );

			handshake_handler_type const& functor =
				boost::bind(
					&client_session::sign_in_on_handshaked,
					this,
					account_id,
					invalid_attempts_count,
					seconds_to_wait
				);
			if ( m_handshaked )
				functor				( );
			else
				handshake			( functor );
		}
	}
	else
		send_message				( invalid_user_name_or_password_message_type, insecure, destroy_after_message_sent );

#ifdef _DEBUG
	LOG								( "AFTER process_sign_in: %d\r\n", s_requests_count );
#endif // #ifdef _DEBUG
}