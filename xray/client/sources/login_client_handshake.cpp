////////////////////////////////////////////////////////////////////////////
//	Created		: 08.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client.h"

using xray::login_client;

void login_client::on_handshake_connected	(
		xray::connection_error_types_enum const connection_result,
		on_handshaked_functor_type const& functor,
		u32 const retry_count
	)
{
	if ( connection_result != successfully_connected ) {
		m_client_state				= signed_out;
		close_connection			( );
		functor						( cannot_handshake );
		return;
	}

	handshake						( functor, retry_count - 1 );
}

void login_client::on_handshaked	(
		boost::system::error_code const& error_code,
		on_handshaked_functor_type const& functor,
		u32 retry_count
	)
{
	ASSERT							( m_connection_state == handshaking );
	if ( error_code ) {
		m_connection_state			= connected;

		if ( retry_count ) {
			LOG						( "NOT handshaked!\r\n" );
			printf					( "error during handshaking: %s\r\n", error_code.message().c_str() );
			handshake				( functor, retry_count - 1 );
		}
		else
			functor					( cannot_handshake );

		return;
	}

	LOG								( "handshaked!\r\n" );
	m_connection_state				= handshaked;
	functor							( successfully_handshaked );
}

void login_client::handshake		( on_handshaked_functor_type const& functor, u32 const retry_count )
{
	if ( m_connection_state == handshaked ) {
		functor						( successfully_handshaked );
		return;
	}

	LOG								( "handshaking...\r\n" );

	ASSERT							( m_connection_state == connected );
	m_connection_state				= handshaking;
#if XRAY_LOGIN_SERVER_USES_SSL
	m_ssl_stream.async_handshake	(
		boost::asio::ssl::stream_base::client,
		boost::bind(
			&login_client::on_handshaked,
			this,
			boost::asio::placeholders::error,
			functor,
			retry_count
		)
	);
#else // #if XRAY_LOGIN_SERVER_USES_SSL
	LOG								( "handshaked!\r\n" );
	m_connection_state				= handshaked;
	functor							( successfully_handshaked );
#endif // #if XRAY_LOGIN_SERVER_USES_SSL
}