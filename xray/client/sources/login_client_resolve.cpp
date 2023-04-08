////////////////////////////////////////////////////////////////////////////
//	Created		: 08.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client.h"

using xray::login_client;

void login_client::on_resolved			(
		tcp::resolver* const resolver,
		u32 const retry_count,
		on_resolved_functor_type const& functor,
		boost::system::error_code const& error_code,
		tcp::resolver::iterator iterator
	)
{
	ASSERT							( m_connection_state == resolving );

	if ( error_code ) {
		LOG							( "NOT resolved!\r\n" );
		LOG							( "error during resolving: %s\r\n", error_code.message().c_str() );
		m_connection_state			= unresolved;
		if ( retry_count ) {
			LOG						( "reconnecting...\r\n" );
			delete					resolver;
			resolve					( functor, retry_count - 1 );
			return;
		}
		else {
			++iterator;
			if ( iterator != tcp::resolver::iterator() ) {
				resolver->async_resolve	(
					*iterator,
					boost::bind(
						&login_client::on_resolved,
						this,
						resolver,
						retry_count,
						functor,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
					)
				);
				return;
			}
		}


		delete						resolver;
		functor						( cannot_resolve, iterator );
		LOG							( "can't resolve endpoints: %s\r\n", error_code.message().c_str() );
		LOG							( "please, try again later\r\n" );
		return;
	}


	delete							resolver;

	LOG								( "resolved!\r\n" );

//	m_ssl_stream.set_verify_callback( boost::asio::ssl::rfc2818_verification( (*iterator).host_name() ) );

	m_connection_state				= resolved;
	functor							( successfully_resolved, iterator );
}

void login_client::resolve			( on_resolved_functor_type const& functor, u32 const retry_count )
{
	LOG								( "resolving...\r\n" );

	ASSERT							( m_connection_state == unresolved );
	m_connection_state				= resolving;

	tcp::resolver* const resolver	= new tcp::resolver( m_io_service );

	char port[6];
	_itoa							( xray::login_tcp_port, port, 10 );

	tcp::resolver::query query( tcp::v4(), m_host, port );
	resolver->async_resolve			(
		query,
		boost::bind(
			&login_client::on_resolved,
			this,
			resolver,
			retry_count,
			functor,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}