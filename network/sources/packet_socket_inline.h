////////////////////////////////////////////////////////////////////////////
//	Created		: 23.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef PACKET_SOCKET_INLINE_H_INCLUDED
#define PACKET_SOCKET_INLINE_H_INCLUDED

namespace xray {
namespace network {

template < typename SocketType >
inline void packet_socket<SocketType>::on_packet_received	( packet const* packet, boost::system::error_code const& error_code, size_t const bytes_transferred )
{
	if ( error_code ) {
		if ( error_code == boost::asio::error::operation_aborted )
			return;

		LOG_ERROR					( "error during reading from socket: %s\r\n", error_code.message().c_str() );
		if ( m_on_error )
			m_on_error				( unable_to_read_from_socket, error_code );

		delete_packet				( packet );
		return;
	}

	if ( bytes_transferred != packet->allocated_size() ) {
		LOG_ERROR					( "unable to read from socket\r\n" );
		if ( m_on_error )
			m_on_error				( unable_to_read_from_socket, error_code );

		delete_packet				( packet );
		return;
	}

	if ( m_on_packet_received )
		m_on_packet_received		( *packet );

	delete_packet					( packet );
	start_receiving					( );
}

template < typename SocketType >
template < typename BufferSizeType >
inline void packet_socket<SocketType>::on_packet_size_received	( boost::system::error_code const& error_code, size_t const bytes_transferred )
{
	if ( error_code ) {
		if ( error_code == boost::asio::error::operation_aborted )
			return;

		LOG_ERROR					( "error during reading from socket: %s\r\n", error_code.message().c_str() );
		if ( m_on_error )
			m_on_error				( unable_to_read_from_socket, error_code );
		return;
	}

	if ( bytes_transferred != sizeof(BufferSizeType) ) {
		LOG_ERROR					( "unable to read from socket\r\n" );
		if ( m_on_error )
			m_on_error				( unable_to_read_from_socket, error_code );
		return;
	}

	BufferSizeType const buffer_size = *static_cast<BufferSizeType const* >( static_cast<pcvoid>(&m_header_buffer) );
	if ( buffer_size ) {
		packet* const packet		= new_packet( );
		packet->resize				( buffer_size );

		boost::asio::async_read		(
			m_socket,
			buffer_to_receive_into( *packet ),
			make_custom_alloc_handler(
				m_allocator,
				boost::bind(
					&packet_socket<SocketType>::on_packet_received,
					this,
					packet,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			)
		);
		return;
	}

	if ( identity(sizeof(BufferSizeType) < sizeof(u16)) )
		boost::asio::async_read			(
			m_socket,
			boost::asio::buffer( &m_header_buffer, sizeof(u16) ),
			make_custom_alloc_handler(
				m_allocator,
				boost::bind(
					&packet_socket<SocketType>::on_packet_size_received<u16>,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			)
		);
	else
		ASSERT						( sizeof(BufferSizeType) < sizeof(u16) );
}

template < typename SocketType >
inline void packet_socket<SocketType>::start_receiving		( )
{
	boost::asio::async_read			(
		m_socket,
		boost::asio::buffer( &m_header_buffer, sizeof(u8) ),
		make_custom_alloc_handler(
			m_allocator,
			boost::bind(
				&packet_socket<SocketType>::on_packet_size_received<u8>,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		)
	);
}

template < typename SocketType >
void packet_socket<SocketType>::on_packet_has_been_sent	(
		packet const* packet_being_sent,
		boost::system::error_code const& error_code,
		size_t const bytes_transferred
	)
{
	delete_packet					( packet_being_sent );

	if ( error_code ) {
		LOG_ERROR					( "error during writing to socket: %s\r\n", error_code.message().c_str() );
		if ( m_on_error )
			m_on_error				( unable_to_write_to_socket, error_code );
		return;
	}

	if ( !bytes_transferred ) {
		LOG_ERROR					( "unable to write to socket\r\n" );
		if ( m_on_error )
			m_on_error				( unable_to_write_to_socket, error_code );
		return;
	}
}

template < typename SocketType >
inline void packet_socket<SocketType>::send			( packet const& packet )
{
	network::packet* cloned_packet	= new_packet( );
	cloned_packet->clone			( packet );
	boost::asio::async_write		(
		m_socket,
		buffer_to_send( *cloned_packet ),
		make_custom_alloc_handler(
			m_allocator,
			boost::bind (
				&packet_socket<SocketType>::on_packet_has_been_sent,
				this,
				cloned_packet,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		)
	);
}

template < typename SocketType >
inline packet* packet_socket<SocketType>::new_packet	( )
{
	return							m_world.new_packet( );
}

template < typename SocketType >
inline void packet_socket<SocketType>::delete_packet	( packet const*& packet )
{
	XRAY_DELETE_IMPL				( g_allocator, packet );
}

template < typename SocketType >
inline void packet_socket<SocketType>::stop_receiving		( )
{
	boost::system::error_code error_code;
	m_socket.cancel					( error_code );
}

} // namespace network
} // namespace xray

#endif // #ifndef PACKET_SOCKET_INLINE_H_INCLUDED