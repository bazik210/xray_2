////////////////////////////////////////////////////////////////////////////
//	Created		: 25.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "client_session.h"

using xray::client_session;

client_session::client_session					(
		boost::asio::io_service& io_service,
		MYSQL& connection
	) :
	m_socket							( io_service ),
	connection_						( connection )
{
}

void client_session::start						( )
{
	read_async						( &client_session::process_client_initiation );
}

void client_session::read_async					( read_handler_type const& read_handler )
{
	m_socket.async_read_some			(
		boost::asio::buffer( data_, max_length ),
		boost::bind(
			read_handler,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void client_session::handle_write				( read_handler_type const& functor, boost::system::error_code const& error )
{
	if ( error || !functor ) {
		delete this;
		return;
	}

	read_async						( functor );
}

void client_session::send_message				( xray::lobby_client_message_types_enum const message_type, read_handler_type const& functor )
{
	u8 const message_type_to_send	= (u8)message_type;
	boost::asio::async_write		(
		m_socket,
		boost::asio::buffer( &message_type_to_send, sizeof(message_type_to_send) ),
		boost::bind (
			functor,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void client_session::send_lobby_info			( )
{
	char const host[]				= "localhost";
	u16 const port					= 5000;
	u32 const buffer_size			= sizeof(host) + 2;
	char* const reply				= (char*)_alloca( buffer_size );
	reply[0]						= sizeof(host) - 1;
	memcpy							( reply + 1, host, reply[0] );
	memcpy							( reply + 1 + reply[0], &port, sizeof(port) );

	boost::asio::async_write		(
		m_socket,
		boost::asio::buffer( reply, buffer_size ),
		boost::bind (
			&client_session::handle_write,
			this,
			read_handler_type(),
			boost::asio::placeholders::error
		)
	);
}

void client_session::add_new_user				( )
{
}

void client_session::process_sign_up			( )
{
	u32 const account_name_length	= data_[1];
	char* const account_name		= (char*)_alloca( (account_name_length + 1) *sizeof(char) );
	memcpy							( account_name, data_ + 1, account_name_length );
	account_name[ account_name_length ]	= 0;
	u32	digest[5];
	memcpy							( digest, data_ + 1 + account_name_length, sizeof(digest) );

	std::string query				= "SELECT account_id,password_hash FROM stalker.accounts WHERE account_name = '" + std::string(account_name) + "'";
	mysql_query						( &connection_, query.c_str() );
	MYSQL_RES* const result			= mysql_store_result( &connection_ );
	if ( MYSQL_ROW const& row = mysql_fetch_row( result ) )
		send_message				( occupied_user_name_message_type, &client_session::process_client_initiation );
	else
		add_new_user				( );

	mysql_free_result				( result );
}

void client_session::process_sign_in			( )
{
	u32 const account_name_length	= data_[1];
	char* const account_name		= (char*)_alloca( (account_name_length + 1) *sizeof(char) );
	memcpy							( account_name, data_ + 1, account_name_length );
	account_name[ account_name_length ]	= 0;
	u32	digest[5];
	memcpy							( digest, data_ + 1 + account_name_length, sizeof(digest) );

	std::string query				= "SELECT account_id FROM stalker.accounts WHERE account_name = '" + std::string(account_name) + "'";
	mysql_query						( &connection_, query.c_str() );
	MYSQL_RES* const result			= mysql_store_result( &connection_ );
	if ( MYSQL_ROW const& row = mysql_fetch_row( result ) ) {
		if ( memcmp( digest, row[1], sizeof(digest) ) != 0 )
			send_message			( invalid_user_name_or_password_message_type, &client_session::process_client_initiation );
		else
			send_lobby_info			( );
	}
	else
		send_message				( invalid_user_name_or_password_message_type, &client_session::process_client_initiation );

	mysql_free_result				( result );
}

void client_session::process_client_initiation	(
		boost::system::error_code const& error,
		size_t const bytes_transferred
	)
{
	if ( !bytes_transferred ) {
		send_message				( lobby_client_invalid_message_type, &client_session::process_client_initiation );
		return;
	}

	if ( error ) {
		delete this;
		return;
	}

	switch ( data_[0] ) {
		case sign_up_message_type : {
			process_sign_up			( );
			break;
		}
		case sign_in_message_type : {
			process_sign_in			( );
			break;
		}
		default : {
			delete this;
			return;
		}
	}
}