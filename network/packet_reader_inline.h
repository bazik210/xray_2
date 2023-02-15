////////////////////////////////////////////////////////////////////////////
//	Created		: 23.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_NETWORK_PACKET_READER_INLINE_H_INCLUDED
#define XRAY_NETWORK_PACKET_READER_INLINE_H_INCLUDED

namespace xray {
namespace network {

inline packet_reader::packet_reader	( packet const& packet ) :
	m_packet	( packet ),
	m_pointer	( packet.buffer() )
{
}

inline void packet_reader::r		( pvoid const destination, u32 const destination_size, u32 const size )
{
	ASSERT_U	( m_pointer >= m_packet.buffer() );
	ASSERT_U	( m_pointer <= m_packet.buffer() + m_packet.buffer_size() );
	ASSERT_U	( m_packet.buffer() + m_packet.buffer_size() >= (m_pointer + size) );

	memory::copy( destination, destination_size, m_pointer, size);
	m_pointer	+= size;
}

template < typename T >
inline T packet_reader::r			( )
{
	T			result;
	r			( &result, sizeof( result ), sizeof( result ) );
	return		( result );
}

inline pstr packet_reader::r_string	( pstr string, u8 const buffer_size )
{
	u8 const string_length	= r<u8>( );
	ASSERT		( string_length < 255 );
	r			( string, buffer_size, string_length );
	string[ string_length ]	= 0;
	return		string;
}

template < int Count >
inline pstr packet_reader::r_string	( char (&string)[Count] )
{
	return		r_string( &string[0], (u8)std::min( sizeof(string), size_t(255) ) );
}

} // namespace network
} // namespace xray

#endif // #ifndef XRAY_NETWORK_PACKET_READER_INLINE_H_INCLUDED