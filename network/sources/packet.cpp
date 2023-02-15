////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "packet_socket.h"

using xray::network::packet;

boost::asio::const_buffers_1 xray::network::buffer_to_send( packet& packet )
{
	u32 const buffer_size	= packet.buffer_size();
	pbyte const buffer		= packet.buffer();
	if ( !buffer_size )
		return				boost::asio::buffer( static_cast<pcvoid>(0), 0 );

	if ( buffer_size < 256 ) {
		*(buffer - 1)		= static_cast<u8>( buffer_size );
		return				boost::asio::buffer( static_cast<pcbyte>(buffer - 1), buffer_size + 1 );
	}

	ASSERT					( buffer_size < (u32(1) << 16) );
	*(buffer - 1)			= 0;
	*static_cast<u16*>(static_cast<pvoid>(buffer - 3))	= static_cast<u16>( buffer_size );
	return					boost::asio::buffer( static_cast<pcbyte>(buffer - 3), buffer_size + 3 );
}

boost::asio::mutable_buffers_1 xray::network::buffer_to_receive_into( packet& packet )
{
	ASSERT					( packet.buffer_size() );
	return					boost::asio::buffer( packet.buffer(), packet.buffer_size() );
}