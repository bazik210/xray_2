////////////////////////////////////////////////////////////////////////////
//	Created		: 08.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client.h"

using xray::login_client;

void login_client::on_ping_sent			(
		u32 const try_count,
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	if ( error_code ) {
		ping						( try_count - 1 );
		printf						( "ping: error during writing to socket: %s\r\n", error_code.message().c_str() );
		return;
	}

	if ( !bytes_transferred ) {
		printf						( "ping: unable to write to socket\r\n" );
		return;
	}

	m_ping_timer.expires_from_now	( boost::posix_time::seconds(1) );
	m_ping_timer.async_wait			( boost::bind( &login_client::ping, this, ping_retry_count ) );
}

void login_client::ping					( u32 retry_count )
{
	if ( m_client_state != signed_in )
		return;

	if ( !retry_count ) {
		m_client_state						= signed_out;
		return;
	}

	m_ping_socket.async_send		(
		boost::asio::buffer( &m_session_id, sizeof(m_session_id) ),
		boost::bind (
			&login_client::on_ping_sent,
			this,
			retry_count,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}