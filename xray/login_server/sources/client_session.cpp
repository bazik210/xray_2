////////////////////////////////////////////////////////////////////////////
//	Created		: 25.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "client_session.h"
#include <boost/uuid/sha1.hpp>
#include "login_server.h"

using xray::client_session;

client_session::client_session		(
		boost::asio::io_service& io_service,
		MYSQL& connection,
#if XRAY_LOGIN_SERVER_USES_SSL
		boost::asio::ssl::context& ssl_context,
#endif // #if XRAY_LOGIN_SERVER_USES_SSL
		xray::login_server& login_server
	) :
	m_socket						( io_service ),
#if XRAY_LOGIN_SERVER_USES_SSL
	m_ssl_stream					( m_socket, ssl_context ),
#else // #if XRAY_LOGIN_SERVER_USES_SSL
	m_ssl_stream					( m_socket ),
#endif // #if XRAY_LOGIN_SERVER_USES_SSL
	m_connection					( connection ),
	m_login_server					( login_server ),
#if XRAY_LOGIN_SERVER_USES_SSL
	m_ssl_context					( ssl_context ),
#endif // #if XRAY_LOGIN_SERVER_USES_SSL
	m_handshaked					( false )
{
}

void client_session::on_handshaked	( boost::system::error_code const& error_code, handshake_handler_type const& functor, u32 const retry_count )
{
	if ( error_code ) {
		LOG_ERROR					( "error during SSL handshaking: %s\r\n", error_code.message().c_str() );
		if ( !retry_count ) {
			disconnect_and_destroy_this	( );
			return;
		}

#if XRAY_LOGIN_SERVER_USES_SSL
		m_ssl_stream.~ssl_stream_type	( );
		new (&m_ssl_stream) ssl_stream_type	( m_socket, m_ssl_context );

		m_ssl_stream.async_handshake	(
			boost::asio::ssl::stream_base::server,
			make_custom_alloc_handler(
				m_allocator,
				boost::bind(
					&client_session::on_handshaked,
					this,
					boost::asio::placeholders::error,
					functor,
					retry_count - 1
				)
			)
		);
#endif // #if XRAY_LOGIN_SERVER_USES_SSL
		return;
	}

	functor							( );
}

void client_session::handshake		( handshake_handler_type const& functor )
{
#if XRAY_LOGIN_SERVER_USES_SSL
	m_ssl_stream.async_handshake	(
		boost::asio::ssl::stream_base::server,
		make_custom_alloc_handler(
			m_allocator,
			boost::bind(
				&client_session::on_handshaked,
				this,
				boost::asio::placeholders::error,
				functor,
				xray::login_handshake_retry_count
			)
		)
	);
#else // #if XRAY_LOGIN_SERVER_USES_SSL
	functor							( );
#endif // #if XRAY_LOGIN_SERVER_USES_SSL
}

void client_session::disconnect_and_destroy_this( )
{
#if XRAY_LOGIN_SERVER_USES_SSL
	m_ssl_stream.~ssl_stream_type	( );
	new (&m_ssl_stream) ssl_stream_type	( m_socket, m_ssl_context );
#endif // #if XRAY_LOGIN_SERVER_USES_SSL

	m_socket.shutdown				( boost::asio::socket_base::shutdown_both );
	m_socket.close					( );

	m_handshaked					= false;
	m_login_server.delete_client_session	( this );
	LOG								( "client has been disconnected\r\n" );
}

void client_session::on_message_sent			(
		xray::login_client_message_types_enum const message_type,
		action_type const action_type,
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	(void)message_type;
//	LOG								( "message has been sent %d\r\n", message_type );

	if ( error_code ) {
		LOG_ERROR					( "error during writing to socket: %s\r\n", error_code.message().c_str() );
		disconnect_and_destroy_this	( );
		return;
	}

	if ( !bytes_transferred ) {
		LOG_ERROR					( "unable to write to socket\r\n" );
		disconnect_and_destroy_this	( );
		return;
	}

	LOG								( "%d bytes have been written\r\n", bytes_transferred );

	switch ( action_type ) {
		case read_after_message_sent : {
			async_read_next_message		( );
			return;
		}
		case destroy_after_message_sent : {
			disconnect_and_destroy_this	( );
			return;
		}
		default : ASSERT( false );
	}
}

void client_session::send_message	(
		xray::login_client_message_types_enum const message_type,
		security_type const write_security_type,
		action_type const action_type
	)
{
	m_data[0]						= (u8)message_type;

	if ( write_security_type == secure )
		boost::asio::async_write	(
			m_ssl_stream,
			boost::asio::buffer( m_data, 1*sizeof(u8) ),
			make_custom_alloc_handler(
				m_allocator,
				boost::bind (
					&client_session::on_message_sent,
					this,
					message_type,
					action_type,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			)
		);
	else
		boost::asio::async_write	(
			m_socket,
			boost::asio::buffer( m_data, 1*sizeof(u8) ),
			make_custom_alloc_handler(
				m_allocator,
				boost::bind (
					&client_session::on_message_sent,
					this,
					message_type,
					action_type,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			)
		);
}

void client_session::add_new_user			( xray::sign_up_info& sign_up_info )
{
	m_login_server.get_queries_processor().add_new_account( sign_up_info );
}

void client_session::update_sign_in_stats	( u32 account_id, u32 invalid_sign_in_attempts_count, boost::posix_time::ptime const& next_sign_in_attempt_time )
{
	m_login_server.get_queries_processor().update_sign_in_stats( account_id, invalid_sign_in_attempts_count, next_sign_in_attempt_time );
}

void client_session::remove_online_user		( u32 const account_id )
{
	m_login_server.get_queries_processor().remove_online_user( account_id );
}

u32 client_session::insert_new_online_user	( u32 account_id )
{
	return m_login_server.get_queries_processor().add_new_online_account( account_id, m_ssl_stream.lowest_layer().remote_endpoint().address().to_string() );
}

void client_session::on_account_name_received	(
		u8 const expected_bytes_count,
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	if ( error_code ) {
		LOG_ERROR					( "error during reading from socket: %s\r\n", error_code.message().c_str() );
		disconnect_and_destroy_this	( );
		return;
	}

	if ( bytes_transferred != expected_bytes_count ) {
		LOG_ERROR					( "unable to read from socket\r\n" );
		disconnect_and_destroy_this	( );
		return;
	}

	LOG								( "%d bytes have been read\r\n", bytes_transferred );

	switch ( m_data[0] ) {
		case sign_up_message_type : {
			process_sign_up			( );
			break;
		}
		case sign_in_message_type : {
			process_sign_in			( );
			break;
		}
		case sign_out_message_type : {
			process_sign_out		( *(u32*)(m_data + 1) );
			break;
		}
		default : {
			LOG						( "unexpected message type\r\n" );
			return;
		}
	}
}

void client_session::on_client_message			(
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	if ( error_code ) {
		LOG_ERROR					( "error during reading from socket: %s\r\n", error_code.message().c_str() );
		disconnect_and_destroy_this	( );
		return;
	}

	if ( bytes_transferred != 2*sizeof(u8) ) {
		LOG_ERROR					( "unable to read from socket\r\n" );
		disconnect_and_destroy_this	( );
		return;
	}

	LOG								( "%d bytes have been read\r\n", bytes_transferred );

	switch ( m_data[0] ) {
		case sign_up_message_type :
		case sign_in_message_type : {
			boost::asio::async_read			(
				m_socket,
				boost::asio::buffer( m_data + 2, m_data[1]*sizeof(u8) ),
				make_custom_alloc_handler(
					m_allocator,
					boost::bind(
						&client_session::on_account_name_received,
						this,
						m_data[1],
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				)
			);
			break;
		}
		case sign_out_message_type : {
			boost::asio::async_read			(
				m_socket,
				boost::asio::buffer( m_data + 2, (sizeof(u32) - 1)*sizeof(u8) ),
				make_custom_alloc_handler(
					m_allocator,
					boost::bind(
						&client_session::on_account_name_received,
						this,
						u8(sizeof(u32) - 1),
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				)
			);
			break;
		}
		default : {
			LOG						( "unexpected message type\r\n" );
			return;
		}
	}
}

void client_session::async_read_next_message	( )
{
	boost::asio::async_read			(
		m_socket,
		boost::asio::buffer( m_data, 2*sizeof(u8) ),
		make_custom_alloc_handler(
			m_allocator,
			boost::bind(
				&client_session::on_client_message,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		)
	);
}

void client_session::start			( )
{
	async_read_next_message			( );
}