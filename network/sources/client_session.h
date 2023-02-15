////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef CLIENT_SESSION_H_INCLUDED
#define CLIENT_SESSION_H_INCLUDED

#include "packet_socket.h"

namespace xray {
namespace network {

class server_impl;

class client_session {
public:
	typedef boost::asio::ip::tcp::socket			socket_type;
	typedef packet_socket< socket_type >			packet_socket_type;
public:
							client_session			(
								boost::asio::io_service& io_service,
								server_impl& server
							);
							~client_session			( );
			void			start					( );
	inline	socket_type&	socket					( ) { return m_socket; }
	inline	void			set_on_packet_received	( packet_socket_type::on_packet_received_type const& functor )	{ m_packet_socket.set_on_packet_received( functor ); }
	inline	void			send					( packet const& packet ) { m_packet_socket.send(packet); }

private:
	// ordering of the next two members is important here
	socket_type				m_socket;
	packet_socket_type		m_packet_socket;

public:
	client_session*			next;
}; // class client_session

} // namespace network
} // namespace xray

#endif // #ifndef CLIENT_SESSION_H_INCLUDED