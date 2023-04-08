////////////////////////////////////////////////////////////////////////////
//	Created		: 23.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef PACKET_SOCKET_H_INCLUDED
#define PACKET_SOCKET_H_INCLUDED

#include "handler_allocator.h"
#include <xray/network/packet.h>
#include "client_error_codes.h"

namespace xray {
namespace network {

class packet;
enum client_error_codes_enum;
class network_world;

template < typename SocketType >
class packet_socket : private boost::noncopyable {
public:
	inline				packet_socket			( SocketType& socket, network_world& world ) : m_socket( socket ), m_world(world) { }
	inline	void		send					( packet const& packet );
	inline	SocketType&	next_layer				( ) const { return m_socket; }
	inline	SocketType&	lowest_layer			( ) const { return m_socket; }
	inline	void		start_receiving			( );
	inline	void		stop_receiving			( );

public:
	typedef boost::function< void ( packet const& packet ) >	on_packet_received_type;
	typedef boost::function<
		void (
			client_error_codes_enum,
			boost::system::error_code
		) >			on_error_type;

public:
	inline	void		set_on_packet_received	( on_packet_received_type const& functor )	{ m_on_packet_received = functor; }
	inline	void		set_on_error			( on_error_type const& functor )			{ m_on_error = functor; }

private:
	inline	void		on_packet_received		( packet const* packet, boost::system::error_code const& error_code, size_t bytes_transferred );
	template < typename BufferSizeType >
	inline	void		on_packet_size_received	( boost::system::error_code const& error_code, size_t bytes_transferred );
	inline	void		on_packet_has_been_sent	(
							packet const* packet_being_sent,
							boost::system::error_code const& error_code,
							size_t bytes_transferred
						);
	inline	packet*		new_packet				( );
	inline	void		delete_packet			( packet const*& packet );

private:
	on_packet_received_type	m_on_packet_received;
	on_error_type			m_on_error;
	handler_allocator				m_allocator;
	SocketType&						m_socket;
	network_world&					m_world;
	u32								m_header_buffer;
}; // class packet_socket

boost::asio::const_buffers_1	buffer_to_send			( packet& packet );
boost::asio::mutable_buffers_1	buffer_to_receive_into	( packet& packet );

} // namespace network
} // namespace xray

#include "packet_socket_inline.h"

#endif // #ifndef PACKET_SOCKET_H_INCLUDED