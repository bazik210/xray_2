////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef SERVER_IMPL_H_INCLUDED
#define SERVER_IMPL_H_INCLUDED

#include "handler_allocator.h"

namespace xray {
namespace network {

enum connection_action_enum;
class packet;
class client_session;
class network_world;

class server_impl : private boost::noncopyable {
public:
	typedef boost::function< connection_action_enum ( client_session& ) >	on_connection_requested_type;
	typedef boost::function< void ( client_session& ) >						on_disconnected_type;
	typedef boost::function< void ( client_session&, packet const& ) >		on_packet_received_type;

public:
					server_impl					( boost::asio::io_service& io_service, network_world& world );
					~server_impl				( );
			void	accept_connections			( u16 port, u16 max_connections_count );
			void	send_broadcast				( packet const& packet );
			void	set_on_connection_requested	( on_connection_requested_type const& functor );
			void	set_on_disconnected			( on_disconnected_type const& functor );
			void	set_on_packet_received		( on_packet_received_type const& functor );
	inline	network_world& world				( ) const { return m_world; }

private:
			void	delete_client_session		( client_session* client_session );
			void	process_client				(
						client_session* new_client,
						boost::system::error_code const& error
					);
			void	start_client_acception		( );

private:
	on_connection_requested_type	m_on_connection_requested;
	on_disconnected_type			m_on_disconnected;
	on_packet_received_type			m_on_packet_received;
	boost::asio::ip::tcp::acceptor	m_client_acceptor;
	handler_allocator				m_socket_allocator;
	boost::asio::io_service&		m_io_service;
	network_world&					m_world;
	client_session*					m_first_client;
	u16								m_max_connections_count;
	u16								m_connections_count;
}; // class server_impl

} // namespace network
} // namespace xray

#endif // #ifndef SERVER_IMPL_H_INCLUDED