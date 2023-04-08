////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "server_impl.h"
#include "client_session.h"
#include "network_world.h"
#include <xray/network/server.h>	// for connection_action_enum only

using xray::network::server_impl;
using boost::asio::ip::tcp;

server_impl::server_impl						( boost::asio::io_service& io_service, xray::network::network_world& world ) :
	m_io_service					( io_service ),
	m_world							( world ),
	m_client_acceptor				( io_service ),
	m_first_client					( 0 ),
	m_max_connections_count			( 0 ),
	m_connections_count				( 0 )
{
}

server_impl::~server_impl						( )
{
	for ( ; m_first_client; ) {
		client_session* const next_client_session	= m_first_client->next; 
		XRAY_DELETE_IMPL			( g_allocator, m_first_client );
		m_first_client				= next_client_session;
	}
}

void server_impl::delete_client_session			( xray::network::client_session* client_session )
{
	XRAY_DELETE_IMPL				( g_allocator, client_session );

	if ( m_connections_count-- >= m_max_connections_count ) {
		if ( m_connections_count < m_max_connections_count )
			start_client_acception	( );
	}
}

void server_impl::process_client				(
		xray::network::client_session* const new_client,
		boost::system::error_code const& error
	)
{
	if ( !error && ( !m_on_connection_requested || (m_on_connection_requested(*new_client) == permit_connection )) ) {
		ASSERT						( !new_client->next );
		new_client->next			= m_first_client;
		m_first_client				= new_client;
		new_client->start			( );
		LOG_INFO					( "new client connected" );
	}
	else
		delete_client_session		( new_client );

	start_client_acception			( );
}

void server_impl::start_client_acception		( )
{
	client_session* const new_client = NEW( client_session) ( m_io_service, *this );
	new_client->set_on_packet_received	( boost::bind( m_on_packet_received, boost::ref(*new_client), _1 ) );
	m_client_acceptor.async_accept	(
		new_client->socket(),
		make_custom_alloc_handler(
			m_socket_allocator,
			boost::bind(
				&server_impl::process_client,
				this,
				new_client,
				boost::asio::placeholders::error
			)
		)
	);
}

void server_impl::accept_connections			( u16 const port, u16 const max_connections_count )
{
	ASSERT							( !m_first_client );
	m_client_acceptor.open			( tcp::v4() );
	m_client_acceptor.bind			( tcp::endpoint( tcp::v4(), port ) );
	m_client_acceptor.listen		( );

	m_max_connections_count			= max_connections_count;

	start_client_acception			( );
}

void server_impl::set_on_connection_requested	( on_connection_requested_type const& functor )
{
	m_on_connection_requested		= functor;
}

void server_impl::set_on_disconnected			( on_disconnected_type const& functor )
{
	m_on_disconnected				= functor;
}

void server_impl::set_on_packet_received		( on_packet_received_type const& functor )
{
	m_on_packet_received			= functor;
	for ( client_session* i = m_first_client; i; i = i->next )
		i->set_on_packet_received	( boost::bind( m_on_packet_received, boost::ref(*i), _1 ) );
}

void server_impl::send_broadcast				( xray::network::packet const& packet )
{
	for ( client_session* i = m_first_client; i; i = i->next )
		(*i).send					( packet );
}