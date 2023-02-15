////////////////////////////////////////////////////////////////////////////
//	Created		: 08.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client.h"

using xray::login_client;

void login_client::on_connected		(
		u32 const retry_count,
		on_connected_functor_type const& functor,
		boost::system::error_code const& error_code,
		tcp::resolver::iterator iterator
	)
{
	ASSERT							( m_connection_state == connecting );

	if ( error_code ) {
		m_connection_state			= resolved;

		if ( retry_count ) {
			LOG						( "NOT connected!\r\n" );
			printf					( "error during connecting: %s\r\n", error_code.message().c_str() );
			LOG						( "reconnecting...\r\n" );
			connect					( successfully_resolved, iterator, retry_count - 1, functor );
		}
		else {
			m_connection_state		= unresolved;
			functor					( cannot_connect );
		}

		return;
	}

	LOG								( "connected!\r\n" );
	m_connection_state				= connected;
	functor							( successfully_connected );
}

void login_client::connect			(
			xray::resolve_error_types_enum error,
			tcp::resolver::iterator iterator,
			u32 const retry_count,
			on_connected_functor_type const& functor
	)
{
	ASSERT							( m_connection_state == resolved );

	if ( error == cannot_resolve ) {
		LOG							( "NOT resolved!\r\n" );
		m_connection_state			= unresolved;
		return;
	}

	LOG								( "connecting...\r\n" );

#if XRAY_LOGIN_SERVER_USES_SSL
//	m_ssl_stream.set_verify_callback	( boost::asio::ssl::rfc2818_verification( (*iterator).host_name() ) );
#endif // #if XRAY_LOGIN_SERVER_USES_SSL

	m_connection_state				= connecting;
	boost::asio::async_connect		(
		m_socket,
		iterator,
		boost::bind(
			&login_client::on_connected,
			this,
			retry_count,
			functor,
			_1,
			_2
		)
	);
}