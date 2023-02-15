////////////////////////////////////////////////////////////////////////////
//	Created		: 23.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_NETWORK_PACKET_READER_H_INCLUDED
#define XRAY_NETWORK_PACKET_READER_H_INCLUDED

#include <xray/network/packet.h>

namespace xray {
namespace network {

class packet_reader : private boost::noncopyable {
public:
	inline	explicit	packet_reader	( packet const& packet );
	inline	void		r				( pvoid result, u32 destination_size, u32 size );
	template < typename T >
	inline	T			r				( );
	template < int Count >
	inline	pstr		r_string		( char (&string)[Count] );
	inline	pstr		r_string		( pstr string, u8 buffer_size );

private:
	packet const&		m_packet;
	pcbyte				m_pointer;
}; // class packet_reader

} // namespace network
} // namespace xray

#include <xray/network/packet_reader_inline.h>

#endif // #ifndef XRAY_NETWORK_PACKET_READER_H_INCLUDED