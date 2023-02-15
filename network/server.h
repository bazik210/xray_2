////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_NETWORK_SERVER_H_INCLUDED
#define XRAY_NETWORK_SERVER_H_INCLUDED

#include <xray/network/api.h>

namespace xray {
namespace network {

class client_session;
class packet;
class packet_reader;
struct world;
class server_impl;
class network_world;

enum connection_action_enum {
	permit_connection,
	forbid_connection,
	delay_decision,
}; // enum connection_action_enum

class XRAY_NETWORK_API server : private core::noncopyable {
public:
	typedef boost::function< void (client_session& client, packet_reader& ) >	server_on_packet_received_type;
	typedef boost::function< connection_action_enum (client_session& client ) >	server_on_connection_requested_type;
	typedef boost::function< void (client_session& client ) >					server_on_disconnected_type;

public:
			server							( world& world );
			~server							( );

	void	start_accepting					( u16 port, u16 max_connections_count );
	void	stop_accepting					( );

	void	send							( xray::network::client_session& client, packet const* packet );
	void	send_broadcast					( packet const* packet );

	void	set_on_packet_received			( server_on_packet_received_type const& on_packet_received );
	void	set_on_connection_requested		( server_on_connection_requested_type const& on_connection_requested );
	void	set_on_disconnected				( server_on_disconnected_type const& on_disconnected );

private:
	connection_action_enum	on_connection_requested		( xray::network::client_session& client );
	connection_action_enum	on_connection_requested_impl( xray::network::client_session& client );
			void	on_disconnected			( xray::network::client_session& client );
			void	on_disconnected_impl	( xray::network::client_session& client );
			void	on_packet_received		( xray::network::client_session& client, xray::network::packet const& packet );
			void	on_packet_received_impl	( xray::network::client_session& client, xray::network::packet const& packet );
			void	create_server			( );
	static	void	destroy_server			( server_impl* server_to_destroy );
			void	start_accepting_impl	( u16 port, u16 max_connections_count );

private:
	server_on_packet_received_type			m_on_packet_received;
	server_on_connection_requested_type		m_on_connection_requested;
	server_on_disconnected_type				m_on_disconnected;
	network_world&							m_world;
	server_impl*							m_server;
}; // class server

} // namespace network
} // namespace xray

#endif // #ifndef XRAY_NETWORK_SERVER_H_INCLUDED