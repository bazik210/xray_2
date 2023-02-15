////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <xray/network/client.h>
#include "network_world.h"
#include "send_order.h"
#include "client_impl.h"
#include "receive_response.h"
#include <xray/network/packet_reader.h>
#include "functor_response.h"
#include "functor_order.h"
#include "connect_order.h"
#include <xray/linkage_helper.h>

XRAY_DECLARE_LINKAGE_ID( network_entry_point )

using xray::network::client;

client::client						( xray::network::world& world ) :
	m_world							( static_cast_checked<network_world&>(world) ),
	m_client						( 0 )
{
	m_world.add_order				(
		XRAY_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &client::create_client, this )
		)
	);
}

client::~client						( )
{
	m_world.add_order				(
		XRAY_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &client::destroy_client, m_client )
		)
	);
}

void client::create_client			( )
{
	ASSERT							( !m_client );
	m_client						= NEW ( client_impl ) ( m_world.io_service(), m_world );
	m_client->set_on_packet_received( boost::bind( &client::on_packet_received, this, _1 ) );
	m_client->set_on_connected		( boost::bind( &client::on_connected, this ) );
	m_client->set_on_disconnected	( boost::bind( &client::on_disconnected, this ) );
}

void client::destroy_client			( xray::network::client_impl* client_to_destroy )
{
	XRAY_DELETE_IMPL				( g_allocator, client_to_destroy );
}

void client::connect_impl			( pcstr const host, u16 const port )
{
	m_client->connect				( host, port );
}

void client::connect				( pcstr const host, u16 const port )
{
	m_world.add_order				(
		XRAY_NEW_IMPL( m_world.orders_allocator(), connect_order ) (
			boost::bind( &client::connect_impl, this, _1, _2 ),
			host,
			port,
			boost::ref(m_world.orders_allocator( ))
		)
	);
}

void client::disconnect				( )
{
	m_world.add_order				(
		XRAY_NEW_IMPL( m_world.orders_allocator(), functor_order ) (
			boost::bind( &client_impl::disconnect, m_client )
		)
	);
}

void client::send					( xray::network::packet const* packet )
{
	m_world.add_order	(
		XRAY_NEW_IMPL( m_world.orders_allocator(), send_order ) (
			boost::bind( &xray::network::client_impl::send, m_client, _1 ),
			boost::cref( *packet ),
			boost::ref(m_world.orders_allocator())
		)
	);
}

void client::on_packet_received_impl( xray::network::packet const& packet )
{
	if ( m_on_packet_received ) {
		packet_reader packet_reader( packet );
		m_on_packet_received	( packet_reader );
	}
}

void client::on_packet_received		( xray::network::packet const& packet )
{
	if ( !m_on_packet_received )
		return;

	xray::network::packet* cloned_packet	= m_world.new_packet( );
	cloned_packet->clone					( packet );
	m_world.add_response					(
		XRAY_NEW_IMPL( m_world.responses_allocator(), receive_response ) (
			boost::bind( &client::on_packet_received_impl, this, _1 ),
			boost::ref( m_world.responses_allocator() ),
			boost::cref( *cloned_packet )
		)
	);
}

void client::set_on_packet_received	( client_on_packet_received_type const& on_packet_received )
{
	m_on_packet_received			= on_packet_received;
}

void client::on_connected_impl		( )
{
	if ( m_on_connected )
		m_on_connected						( );
}

void client::on_connected			( )
{
	if ( !m_on_connected )
		return;

	m_world.add_response					(
		XRAY_NEW_IMPL( m_world.responses_allocator(), functor_response ) (
			boost::bind( &client::on_connected_impl, this )
		)
	);
}

void client::set_on_connected	( client_on_disconnected_type const& on_connected )
{
	m_on_connected				= on_connected;
}

void client::on_disconnected_impl	( )
{
	if ( m_on_disconnected )
		m_on_disconnected					( );
}

void client::on_disconnected		( )
{
	if ( !m_on_disconnected )
		return;

	m_world.add_response					(
		XRAY_NEW_IMPL( m_world.responses_allocator(), functor_response ) (
			boost::bind( &client::on_disconnected_impl, this )
		)
	);
}

void client::set_on_disconnected	( client_on_disconnected_type const& on_disconnected )
{
	m_on_disconnected				= on_disconnected;
}