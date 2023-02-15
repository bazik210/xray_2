////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_world.h"
#include <xray/network/packet.h>
#include <xray/network/engine.h>
#include "functor_response.h"
#include "functor_order.h"

using xray::network::network_world;

static void empty_function ( ) { }

network_world::network_world					( xray::network::engine& engine, xray::memory::base_allocator& orders_allocator ) :
	m_engine							( engine ),
	m_channel							( *xray::network::g_allocator, orders_allocator )
{
	m_channel.orders.user_initialize	( );
	m_channel.responses.owner_initialize( NEW(functor_response)( &empty_function ), NEW(functor_response)( &empty_function ) );
}

network_world::~network_world					( )
{
	m_channel.responses.owner_finalize	( );
}

void network_world::initialize					( )
{
	m_channel.orders.owner_initialize	( XRAY_NEW_IMPL(orders_allocator(), functor_order)( &empty_function ), XRAY_NEW_IMPL(orders_allocator(), functor_order)( &empty_function ) );
	m_channel.responses.user_initialize	( );
}

void network_world::finalize					( )
{
	m_channel.orders.owner_finalize		( );
}

void network_world::tick						( )
{
	process_orders						( );
	m_io_service.poll					( );
}

void network_world::add_order					( xray::network::order* const order )
{
	m_channel.orders.owner_push_back	( order );
}

void network_world::add_response				( xray::network::response* const response )
{
	m_channel.responses.owner_push_back	( response );
}

void network_world::process_orders				( )
{
	m_channel.responses.owner_delete_processed_items	( );
	while ( order* const order = m_channel.orders.user_pop_front( ) )
		order->execute					( );
}

void network_world::process_responses			( )
{
	m_channel.orders.owner_delete_processed_items	( );
	while ( response* const response = m_channel.responses.user_pop_front() )
		response->execute				( );
}

void network_world::clear_resources				( )
{
}

void network_world::dispatch_callbacks			( )
{
	process_responses					( );
}

xray::network::packet* network_world::new_packet( )
{
	return								NEW( packet ) ( *g_allocator );
}