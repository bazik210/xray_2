////////////////////////////////////////////////////////////////////////////
//	Created		: 27.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "client.h"
#include "connection_info.h"

using xray::client;

client::client						( boost::asio::io_service& io_service, pcstr const login_host ) :
	m_login_client					( io_service, login_host ),
	m_io_service					( io_service ),
	m_error_description				( 0 ),
	m_is_signed_in					( false )
{
}

client::~client						( )
{
	if ( !m_is_signed_in )
		return;

	sign_out						( );

	do {
		m_io_service.run_one		( );
	} while ( !m_login_client.is_signed_out() && !has_last_command_failed() );	
}

void client::update_signed_in_state	( )
{
	m_is_signed_in					= m_login_client.is_signed_in() && m_lobby_client.is_connected();
}

void client::on_lobby_connected		( xray::lobby_client_message_types_enum const message_type )
{
	switch ( message_type ) {
		case connection_successful : {
			update_signed_in_state	( );
			break;
		}
		case invalid_session_id : {
			m_error_description		= "lobby: invalid session id";
			m_is_signed_in			= false;
			sign_out				( );
			break;
		}
		default : {
			m_error_description		= "lobby: unexpected message type";
			m_is_signed_in			= false;
			sign_out				( );
			break;
		}
	}
}

void client::on_signed_in			(
		xray::connection_error_types_enum const connection_error,
		xray::handshaking_error_types_enum const handshaking_error,
		xray::socket_error_types_enum const socket_error,
		xray::login_client_message_types_enum const message_type,
		xray::connection_info const& connection_info
	)
{
	switch ( connection_error ) {
		case successfully_connected : {
			break;
		}
		case cannot_connect : {
			m_error_description		= "login: cannot connect to server";
			return;
		}
		default : {
			m_error_description		= "login: unexpected socket error type";
			return;
		}
	}

	switch ( handshaking_error ) {
		case successfully_handshaked :	break;
		case cannot_handshake : {
			m_error_description		= "login: SSL certificate verification failed";
			return;
		}
		case no_handshake :				break;
		default : {
			m_error_description		= "login: unexpected SSL error";
			return;
		}
	}

	switch ( socket_error ) {
		case no_socket_error : {
			break;
		}
		case unable_to_write_to_socket : {
			m_error_description		= "login: unable to write to socket";
			return;
		}
		case unable_to_read_from_socket : {
			m_error_description		= "login: unable to read from socket";
			return;
		}
		default : {
			m_error_description		= "login: unexpected socket error type";
			return;
		}
	}

	switch ( message_type ) {
		case lobby_info_message_type : {
			ASSERT					( !m_error_description );

//			m_security_client.connect ( connection_info.security, boost::bind(&client::on_security_connected, this, _1) );
			m_lobby_client.connect	( connection_info.lobby, boost::bind(&client::on_lobby_connected, this, _1) );
			m_is_signed_in			= true;
//			m_chat_client.connect	( connection_info.chat, boost::bind(&client::on_chat_connected, this, _1) );

			break;
		}
		case invalid_user_name_or_password_message_type : {
			m_error_description		= "invalid user name or password";
			break;
		}
		case sign_in_attempt_interval_violated_message_type : {
			m_error_description		= "sign in attempt interval is violated";
			break;
		}
		default : {
			m_error_description		= "login: unexpected message type";
			break;
		}
	}
}

void client::sign_in				( pcstr const account_name, pcstr const password )
{
	ASSERT							( !m_is_signed_in );
	m_error_description				= 0;
	m_login_client.sign_in			( account_name, password, boost::bind(&client::on_signed_in, this, _1, _2, _3, _4, _5) );
}

void client::on_signed_out			(
		xray::connection_error_types_enum const connection_error, 
		xray::handshaking_error_types_enum const handshaking_error,
		xray::socket_error_types_enum socket_error,
		xray::login_client_message_types_enum message_type
	)
{
	switch ( connection_error ) {
		case successfully_connected : {
			break;
		}
		case cannot_connect : {
			m_error_description		= "login: cannot connect to server";
			m_is_signed_in			= false;
			return;
		}
		default : {
			m_error_description		= "login: unexpected socket error type";
			m_is_signed_in			= false;
			return;
		}
	}

	switch ( handshaking_error ) {
		case successfully_handshaked :	break;
		case cannot_handshake : {
			m_error_description		= "login: SSL certificate verification failed";
			return;
		}
		case no_handshake :				break;
		default : {
			m_error_description		= "login: unexpected SSL error";
			return;
		}
	}

	switch ( socket_error ) {
		case no_socket_error : {
			break;
		}
		case unable_to_write_to_socket : {
			m_error_description		= "login: unable to write to socket";
			return;
		}
		case unable_to_read_from_socket : {
			m_error_description		= "login: unable to read from socket";
			return;
		}
		default : {
			m_error_description		= "login: unexpected socket error type";
			return;
		}
	}

	switch ( message_type ) {
		case sign_out_successful : {
			ASSERT					( !m_error_description );
			break;
		}
		default : {
			m_error_description		= "login: unexpected message type";
			break;
		}
	}

	m_is_signed_in					= false;
}

void client::sign_out				( )
{
	m_error_description				= 0;
	m_login_client.sign_out			( boost::bind(&client::on_signed_out, this, _1, _2, _3, _4) );
}

void client::on_signed_up			(
		xray::connection_error_types_enum const connection_error,
		xray::handshaking_error_types_enum const handshaking_error,
		xray::socket_error_types_enum const socket_error,
		xray::login_client_message_types_enum const message_type,
		xray::sign_up_info const& sign_up_info,
		bool& finished
	)
{
	(void)&sign_up_info;
	finished						= true;

	switch ( connection_error ) {
		case successfully_connected : {
			break;
		}
		case cannot_connect : {
			m_error_description		= "login: cannot connect to server";
			m_is_signed_in			= false;
			return;
		}
		default : {
			m_error_description		= "login: unexpected socket error type";
			m_is_signed_in			= false;
			return;
		}
	}

	switch ( handshaking_error ) {
		case successfully_handshaked :	break;
		case cannot_handshake : {
			m_error_description		= "login: SSL certificate verification failed";
			return;
		}
		case no_handshake :				break;
		default : {
			m_error_description		= "login: unexpected SSL error";
			return;
		}
	}

	switch ( socket_error ) {
		case no_socket_error :		break;
		case unable_to_write_to_socket : {
			m_error_description		= "login: unable to write to socket";
			return;
		}
		case unable_to_read_from_socket : {
			m_error_description		= "login: unable to read from socket";
			return;
		}
		default : {
			m_error_description		= "login: unexpected socket error";
			return;
		}
	}

	switch ( message_type ) {
		case sign_up_successful_message_type : {
			ASSERT					( !m_error_description );
			break;
		}
		case occupied_user_name_message_type : {
			m_error_description		= "user name has already been occupied, try another one";
			break;
		}
		default : {
			m_error_description		= "login: unexpected message type";
			break;
		}
	}
}

void client::sign_up				( xray::sign_up_info const& sign_up_info, bool& finished )
{
	ASSERT							( !m_is_signed_in );

	m_error_description				= 0;
	finished						= false;
	m_login_client.sign_up			( sign_up_info, boost::bind(&client::on_signed_up, this, _1, _2, _3, _4, _5, boost::ref(finished)) );
}

void client::set_status		( status_enum const status )
{
	m_error_description				= 0;
}