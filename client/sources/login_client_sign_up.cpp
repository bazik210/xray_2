////////////////////////////////////////////////////////////////////////////
//	Created		: 08.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client.h"
#include <xray/login_server/login_structures.h>

using xray::login_client;

void login_client::on_sign_up_answer_received	(
		sign_up_callback_type const& callback,
		xray::sign_up_info const& sign_up_info,
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	if ( error_code ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, successfully_handshaked, unable_to_read_from_socket, login_client_invalid_message_type, sign_up_info );
		printf						( "on_SIGN_UP_answer_received: error during reading from socket: %s\r\n", error_code.message().c_str() );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, successfully_handshaked, unable_to_read_from_socket, login_client_invalid_message_type, sign_up_info );
		return;
	}

	m_client_state					= signed_out;
	close_connection				( );
	callback						( successfully_connected, successfully_handshaked, no_socket_error, (login_client_message_types_enum)m_data[0], sign_up_info );
}

void login_client::on_sign_up_info_written	(
		sign_up_callback_type const& callback,
		xray::sign_up_info const& sign_up_info,
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	ASSERT							( m_client_state == signing_up );

	if ( error_code ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_client_invalid_message_type, sign_up_info );
		printf						( "on_SIGN_UP_password_written: error during writing to socket: %s\r\n", error_code.message().c_str() );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_client_invalid_message_type, sign_up_info );
		return;
	}

	m_ssl_stream.async_read_some	(
		boost::asio::buffer( m_data, 1*sizeof(u8) ),
		boost::bind(
			&login_client::on_sign_up_answer_received,
			this,
			callback,
			boost::cref( sign_up_info ),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void login_client::sign_up_on_handshaked	(
		sign_up_callback_type const& callback,
		xray::sign_up_info const& sign_up_info,
		xray::handshaking_error_types_enum const handshaking_result
	)
{
	if ( handshaking_result != successfully_handshaked ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, handshaking_result, invalid_socket_error_type, login_client_invalid_message_type, sign_up_info );
		return;
	}

	pbyte buffer					= m_data;

	u8 const password_length		= (u8)strlen(sign_up_info.password);
	ASSERT							( password_length < max_password_length );
	*buffer++						= password_length;
	memcpy							( buffer, sign_up_info.password, password_length );
	buffer							+= password_length;

	u8 const email_length			= (u8)strlen(sign_up_info.email);
	ASSERT							( email_length < max_email_length );
	*buffer++						= email_length;
	memcpy							( buffer, sign_up_info.email, email_length );
	buffer							+= email_length;

	boost::asio::async_write		(
		m_ssl_stream,
		boost::asio::buffer( m_data, buffer - m_data ),
		boost::bind(
			&login_client::on_sign_up_info_written,
			this,
			callback,
			boost::cref( sign_up_info ),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void login_client::on_sign_up_account_answer_received	(
		sign_up_callback_type const& callback,
		xray::sign_up_info const& sign_up_info,
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	if ( error_code ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, successfully_handshaked, unable_to_read_from_socket, login_client_invalid_message_type, sign_up_info );
		printf						( "on_SIGN_UP_answer_received: error during reading from socket: %s\r\n", error_code.message().c_str() );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, successfully_handshaked, unable_to_read_from_socket, login_client_invalid_message_type, sign_up_info );
		return;
	}

	switch ( m_data[0] ) {
		case occupied_user_name_message_type : {
			m_client_state			= signed_out;
			close_connection		( );
			callback				( successfully_connected, successfully_handshaked, no_socket_error, (login_client_message_types_enum)m_data[0], sign_up_info );
			return;
		}
		case send_sign_up_info_message_type : {
			break;
		}
		default : {
			m_client_state			= signed_out;
			close_connection		( );
			callback				( successfully_connected, successfully_handshaked, no_socket_error, (login_client_message_types_enum)m_data[0], sign_up_info );
			return;
		}
	}

	handshake						(
		boost::bind(
			&login_client::sign_up_on_handshaked,
			this,
			callback,
			boost::cref( sign_up_info ),
			_1
		),
		login_handshake_retry_count
	);
}

void login_client::on_sign_up_written		(
		sign_up_callback_type const& callback,
		xray::sign_up_info const& sign_up_info,
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	ASSERT							( m_client_state == signing_up );

	if ( error_code ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_client_invalid_message_type, sign_up_info );
		printf						( "SIGN_UP: error during writing to socket: %s\r\n", error_code.message().c_str() );
		return;
	}

	if ( !bytes_transferred ) {
		m_client_state				= signed_out;
		close_connection			( );
		callback					( successfully_connected, successfully_handshaked, unable_to_write_to_socket, login_client_invalid_message_type, sign_up_info );
		return;
	}

	m_socket.async_read_some	(
		boost::asio::buffer( m_data, 1*sizeof(u8) ),
		boost::bind(
			&login_client::on_sign_up_account_answer_received,
			this,
			callback,
			boost::cref( sign_up_info ),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void login_client::sign_up_on_connected	(
		xray::connection_error_types_enum const connection_result,
		sign_up_callback_type const& callback,
		xray::sign_up_info const& sign_up_info
	)
{
	if ( connection_result != successfully_connected ) {
		m_client_state				= signed_out;
		callback					( cannot_connect, handshaking_error_type, invalid_socket_error_type, login_client_invalid_message_type, sign_up_info );
		return;
	}

	m_client_state					= signing_up;

	pbyte buffer					= m_data;

	*buffer++						= xray::sign_up_message_type;

	u8 const account_name_length	= (u8)strlen(sign_up_info.account_name);
	ASSERT							( account_name_length < max_account_name_length );
	*buffer++						= account_name_length;
	memcpy							( buffer, sign_up_info.account_name, account_name_length );
	buffer							+= account_name_length;

	boost::asio::async_write		(
		m_socket,
		boost::asio::buffer( m_data, buffer - m_data ),
		boost::bind(
			&login_client::on_sign_up_written,
			this,
			callback,
			boost::cref( sign_up_info ),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void login_client::sign_up			( xray::sign_up_info const& sign_up_info, sign_up_callback_type const& callback )
{
	establish_connection			(
		boost::bind (
			&login_client::sign_up_on_connected,
			this,
			_1,
			callback,
			boost::cref( sign_up_info )
		),
		login_resolve_retry_count,
		login_connect_retry_count
	);
}