////////////////////////////////////////////////////////////////////////////
//	Created		: 08.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client.h"
#include "connection_info.h"

using xray::login_client;

void login_client::on_sign_in_answer_received	(
		sign_in_callback_type const& callback, 
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	ASSERT							( m_client_state == signing_in );

	if ( error_code || !bytes_transferred ) {
		m_client_state				= signed_out;
		close_connection			( );
		if ( error_code )
			printf					( "error during reading sign in answer: %s\r\n", error_code.message().c_str() );
		callback					( successfully_connected, successfully_handshaked, no_socket_error, sign_in_attempt_interval_violated_message_type, connection_info() );
		return;
	}

	LOG								( "answer has been received!\r\n" );
	pbyte buffer					= m_data;
	switch ( *buffer ) {
		case xray::lobby_info_message_type : {
			++buffer;
			u8 host_name_length		= std::min( *buffer++, u8(max_host_name_length - 1) );

			xray::connection_info connection_info;
			memcpy					( connection_info.lobby.host, buffer, host_name_length*sizeof(char) );
			buffer					+= host_name_length*sizeof(char);
			connection_info.lobby.host[ host_name_length ] = 0;

			memcpy					( &connection_info.lobby.session_id, buffer, sizeof(connection_info.lobby.session_id) );
			buffer					+= sizeof(connection_info.lobby.session_id);

			m_session_id			= connection_info.lobby.session_id;

			char port[6];
			_itoa					( login_udp_port, port, 10 );

			udp::resolver resolver( m_io_service );
			udp::resolver::query query(
				m_socket.remote_endpoint().address().is_v4() ? udp::v4() : udp::v6(),
				m_socket.remote_endpoint().address().to_string(),
				port
			);
			udp::resolver::iterator iterator = resolver.resolve(query);
			m_ping_socket.connect	( *iterator );

			m_client_state			= signed_in;
			close_connection		( );
			callback				( successfully_connected, successfully_handshaked, no_socket_error, lobby_info_message_type, connection_info );

			m_ping_timer.async_wait	( boost::bind( &login_client::ping, this, ping_retry_count ) );
			return;
		}
		default : {
			m_client_state			= signed_out;
			callback				( successfully_connected, successfully_handshaked, no_socket_error, login_client_message_types_enum(buffer[0]), connection_info() );
			return;
		}
	}
}

void login_client::on_sign_in_password_written	(
		sign_in_callback_type const& callback,
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	if ( error_code ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_client_invalid_message_type, connection_info() );
		printf						( "write_password_during_SIGN_IN: error during writing to socket: %s\r\n", error_code.message().c_str() );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_client_invalid_message_type, connection_info() );
		return;
	}

	LOG								( "password has been written!\r\n" );
	LOG								( "waiting for answer...\r\n" );
	u32 const buffer_size			= 1 + 1 + max_host_name_length + sizeof(lobby_connection_info().session_id);
	m_ssl_stream.async_read_some	(
		boost::asio::buffer(
			m_data,
			buffer_size
		),
		boost::bind(
			&login_client::on_sign_in_answer_received,
			this,
			callback,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void login_client::on_sign_in_handshaked( sign_in_callback_type const& callback, xray::handshaking_error_types_enum const error )
{
	if ( error == cannot_handshake ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, cannot_handshake, no_socket_error, sign_in_attempt_interval_violated_message_type, connection_info() );
		return;
	}

	LOG								( "writing password...\r\n" );
	u32 const password_length		= strlen( m_password );
	pbyte buffer					= m_data;
	*buffer++						= (u8)password_length;
	memcpy							( buffer, m_password, password_length*sizeof(char) );
	buffer							+= password_length;

	boost::asio::async_write		(
		m_ssl_stream,
		boost::asio::buffer( m_data, buffer - m_data ),
		boost::bind(
			&login_client::on_sign_in_password_written,
			this,
			callback,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void login_client::on_sign_in_written	( sign_in_callback_type const& callback, boost::system::error_code const& error_code, size_t const bytes_transferred )
{
	ASSERT							( m_client_state == signing_in );

	if ( error_code ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, no_handshake, unable_to_write_to_socket, login_client_invalid_message_type, connection_info() );
		printf						( "SIGN_IN: error during writing to socket: %s\r\n", error_code.message().c_str() );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, no_handshake, unable_to_write_to_socket, login_client_invalid_message_type, connection_info() );
		return;
	}

	handshake						(
		boost::bind(
			&login_client::on_sign_in_handshaked,
			this,
			callback,
			_1
		),
		login_handshake_retry_count
	);
}

void login_client::sign_in_on_connected	( xray::connection_error_types_enum const connection_result, sign_in_callback_type const& callback )
{
	if ( connection_result != successfully_connected ) {
		m_client_state				= signed_out;
		callback					( cannot_connect, no_handshake, no_socket_error, login_client_invalid_message_type, connection_info() );
		return;
	}

	pbyte buffer					= m_data;

	*buffer++						= xray::sign_in_message_type;

	u8 const account_name_length	= (u8)strlen(m_account_name);
	ASSERT							( account_name_length < max_account_name_length );
	*buffer++						= account_name_length;
	memcpy							( buffer, m_account_name, account_name_length );
	buffer							+= account_name_length;

	m_client_state					= signing_in;

	boost::asio::async_write		(
		m_socket,
		boost::asio::buffer( m_data, buffer - m_data ),
		boost::bind(
			&login_client::on_sign_in_written,
			this,
			callback,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void login_client::sign_in			( pcstr const account_name, pcstr const password, sign_in_callback_type const& callback )
{
	strncpy							( m_account_name, account_name, sizeof(m_account_name) );
	m_account_name[ sizeof(m_account_name) - 1 ] = 0;

	strncpy							( m_password, password, sizeof(m_password) );
	m_password[ sizeof(m_password) - 1 ] = 0;

	on_connected_functor_type const& sign_in_functor =
		boost::bind (
			&login_client::sign_in_on_connected,			
			this,
			_1,
			callback
		);
	
	LOG								( "signing in...\r\n" );

	if ( m_connection_state == unresolved )
		establish_connection		( sign_in_functor, login_resolve_retry_count, login_connect_retry_count );
	else if ( m_client_state == signed_out )
		sign_in_functor				( successfully_connected );
	else
		printf						( "waiting for previous operation to be completed\r\n" );
}