////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_WORLD_H_INCLUDED
#define NETWORK_WORLD_H_INCLUDED

#include <xray/network/world.h>
#include "two_way_threads_channel.h"
#include <xray/network/engine.h>

namespace xray {
namespace network {

struct engine;
class packet;
class order;
class packet;
class server_impl;

class network_world :
	public network::world,
	private boost::noncopyable
{
public:
					network_world		( network::engine& engine, memory::base_allocator& orders_allocator );
	virtual			~network_world		( );
	virtual	void	initialize			( );
	virtual	void	finalize			( );
	virtual	void	tick				( );
	virtual	void	clear_resources		( );
	virtual	void	dispatch_callbacks	( );

public:
			void	add_order			( order* order );
			void	add_response		( response* response );
			packet* new_packet			( );

public:
	inline	memory::base_allocator&		orders_allocator	( ) const	{ return m_channel.orders.owner_allocator(); }
	inline	memory::doug_lea_allocator& responses_allocator	( ) const	{ return *::xray::network::g_allocator; }
	inline	boost::asio::io_service&	io_service			( )			{ return m_io_service; }

private:
			void	process_orders		( );
			void	process_responses	( );

private:
	boost::asio::io_service m_io_service;
	two_way_threads_channel	m_channel;
	engine&					m_engine;
}; // class network_world

} // namespace network
} // namespace xray

#endif // #ifndef NETWORK_WORLD_H_INCLUDED