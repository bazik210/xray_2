////////////////////////////////////////////////////////////////////////////
//	Created		: 08.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client.h"

using xray::login_client;

void login_client::on_sign_out_password_written	(
		sign_out_callback_type const& callback,
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	if ( error_code ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_client_invalid_message_type );
		printf						( "write_password_during_sign_OUT: error during writing to socket: %s\r\n", error_code.message().c_str() );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_client_invalid_message_type );
		return;
	}

	LOG								( "on sign out password written\r\n" );

	m_client_state					= signed_out;
	close_connection				( );
	callback						( successfully_connected, successfully_handshaked, no_socket_error, lobby_info_message_type );
}

void login_client::on_sign_out_handshaked( sign_out_callback_type const& callback, xray::handshaking_error_types_enum const error )
{
	if ( error == cannot_handshake ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, cannot_handshake, no_socket_error, lobby_info_message_type );
		return;
	}

	LOG								( "on sign out handshaked\r\n" );

	u32 const password_length		= strlen( m_password );
	pbyte buffer					= m_data;
	*buffer++						= (u8)password_length;
	memcpy							( buffer, m_password, password_length*sizeof(char) );
	buffer							+= password_length;

	boost::asio::async_write		(
		m_ssl_stream,
		boost::asio::buffer( m_data, buffer - m_data ),
		boost::bind(
			&login_client::on_sign_out_password_written,
			this,
			callback,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void login_client::on_sign_out_written	( sign_out_callback_type const& callback, boost::system::error_code const& error_code, size_t const bytes_transferred )
{
	ASSERT							( m_client_state == signing_out );

	if ( error_code ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, no_handshake, unable_to_write_to_socket, login_client_invalid_message_type );
		printf						( "SIGN_OUT: error during writing to socket: %s\r\n", error_code.message().c_str() );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, no_handshake, unable_to_write_to_socket, login_client_invalid_message_type );
		return;
	}

	LOG								( "on sign out written\r\n" );

	handshake						(
		boost::bind(
			&login_client::on_sign_out_handshaked,
			this,
			callback,
			_1
		),
		login_handshake_retry_count
	);
}

void login_client::sign_out_on_connected	( xray::connection_error_types_enum const connection_result, sign_out_callback_type const& callback )
{
	if ( connection_result != successfully_connected ) {
		m_client_state				= signed_out;
		callback					( cannot_connect, no_handshake, no_socket_error, login_client_invalid_message_type );
		return;
	}

	LOG								( "sign out - on connected\r\n" );

	pbyte buffer					= m_data;

	*buffer++						= xray::sign_out_message_type;

	memcpy							( buffer, &m_session_id, sizeof(m_session_id) );
	buffer							+= sizeof(m_session_id);

	m_client_state					= signing_out;

	boost::asio::async_write		(
		m_socket,
		boost::asio::buffer( m_data, buffer - m_data ),
		boost::bind(
			&login_client::on_sign_out_written,
			this,
			callback,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void login_client::sign_out			( sign_out_callback_type const& callback )
{
	on_connected_functor_type const& sign_out_functor =
		boost::bind (
			&login_client::sign_out_on_connected,
			this,
			_1,
			callback
		);
	
	LOG								( "signing out...\r\n" );

	if ( m_client_state == signed_out )
		printf						( "client has already been signed out\r\n" );
	else if ( m_client_state == signed_in ) {
		if ( m_connection_state == unresolved )
			establish_connection	( sign_out_functor, login_resolve_retry_count, login_connect_retry_count );
		else
			sign_out_functor		( successfully_connected );
	}
	else
		printf						( "waiting for previous operation to be completed\r\n" );
}