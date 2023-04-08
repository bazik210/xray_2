////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <xray/network/server.h>
#include "network_world.h"
#include "send_order.h"
#include "receive_response.h"
#include "client_session.h"
#include <xray/network/packet_reader.h>
#include <xray/network/packet.h>
#include "server_impl.h"
#include "functor_order.h"
#include "functor_response.h"

using xray::network::server;

server::server							( xray::network::world& world ) :
	m_world							( static_cast_checked<network_world&>(world) ),
	m_server						( 0 )
{
	m_world.add_order				(
		XRAY_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &server::create_server, this )
		)
	);
}

server::~server							( )
{
	m_world.add_order				(
		XRAY_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &server::destroy_server, m_server )
		)
	);
}

void server::on_packet_received_impl	( xray::network::client_session& client, xray::network::packet const& packet )
{
	if ( m_on_packet_received ) {
		packet_reader packet_reader( packet );
		m_on_packet_received		( client, packet_reader );
	}
}

void server::on_packet_received			( xray::network::client_session& client, xray::network::packet const& packet )
{
	if ( !m_on_packet_received )
		return;

	network::packet* const cloned_packet		= m_world.new_packet( );
	cloned_packet->clone			( packet );
	m_world.add_response			(
		XRAY_NEW_IMPL( m_world.responses_allocator(), receive_response ) (
			boost::bind( &server::on_packet_received_impl, this, boost::ref(client), _1 ),
			boost::ref( m_world.responses_allocator() ),
			boost::cref( *cloned_packet )
		)
	);
}

xray::network::connection_action_enum server::on_connection_requested_impl	( xray::network::client_session& client )
{
	if ( m_on_connection_requested )
		return						m_on_connection_requested( client );

	return							permit_connection;
}

xray::network::connection_action_enum server::on_connection_requested		( xray::network::client_session& client )
{
	return							permit_connection;
	//if ( !m_on_connection_requested )
	//	return						permit_connection;

	//m_world.add_response			(
	//	XRAY_NEW_IMPL( m_world.responses_allocator(), functor_response ) (
	//		boost::bind( &server::on_connection_requested_impl, this, boost::ref(client) )
	//	)
	//);
}

void server::on_disconnected_impl	( xray::network::client_session& client )
{
	if ( m_on_disconnected )
		m_on_disconnected			( client );
}

void server::on_disconnected		( xray::network::client_session& client )
{
	if ( !m_on_disconnected )
		return;

	m_world.add_response			(
		XRAY_NEW_IMPL( m_world.responses_allocator(), functor_response ) (
			boost::bind( &server::on_disconnected_impl, this, boost::ref(client) )
		)
	);
}

void server::create_server				( )
{
	ASSERT							( !m_server );
	m_server						= NEW ( server_impl ) ( m_world.io_service(), m_world );
	m_server->set_on_packet_received		( boost::bind( &server::on_packet_received, this, _1, _2 ) );
	m_server->set_on_connection_requested	( boost::bind( &server::on_connection_requested, this, _1 ) );
	m_server->set_on_disconnected			( boost::bind( &server::on_disconnected, this, _1 ) );
}

void server::destroy_server				( xray::network::server_impl* server_to_destroy )
{
	XRAY_DELETE_IMPL				( g_allocator, server_to_destroy );
}

void server::start_accepting_impl		( u16 port, u16 max_connections_count )
{
	m_server->accept_connections	( port, max_connections_count );
}

void server::start_accepting			( u16 const port, u16 const max_connections_count )
{
	m_world.add_order				(
		XRAY_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &server::start_accepting_impl, this, port, max_connections_count )
		)
	);
}

void server::stop_accepting				( )
{
	NOT_IMPLEMENTED					( );
}

void server::send						( xray::network::client_session& client, xray::network::packet const* const packet )
{
	m_world.add_order				(
		XRAY_NEW_IMPL( m_world.orders_allocator(), send_order ) (
			boost::bind( &xray::network::client_session::send, &client, _1 ),
			boost::cref( *packet ),
			boost::ref( m_world.orders_allocator() )
		)
	);
}

void server::send_broadcast				( xray::network::packet const* const packet )
{
	m_world.add_order				(
		XRAY_NEW_IMPL( m_world.orders_allocator(), send_order ) (
			boost::bind( &xray::network::server_impl::send_broadcast, m_server, _1 ),
			boost::cref( *packet ),
			boost::ref( m_world.orders_allocator() )
		)
	);
}

void server::set_on_packet_received		( server::server_on_packet_received_type const& on_packet_received )
{
	m_on_packet_received			= on_packet_received;
}

void server::set_on_connection_requested( server_on_connection_requested_type const& on_connection_requested )
{
	m_on_connection_requested		= on_connection_requested;
}

void server::set_on_disconnected		( server_on_disconnected_type const& on_disconnected )
{
	m_on_disconnected				= on_disconnected;
}