////////////////////////////////////////////////////////////////////////////
//	Created		: 21.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_NETWORK_PACKET_INLINE_H_INCLUDED
#define XRAY_NETWORK_PACKET_INLINE_H_INCLUDED

namespace xray {
namespace network {

inline packet::packet			( memory::base_allocator& allocator ) :
	next				( 0 ),
	m_buffer			( 0 ),
	m_allocator			( allocator ),
	m_buffer_size		( 0 ),
	m_allocated_size	( 0 )
{
}

inline packet::~packet			( )
{
	if ( m_buffer ) {
		pbyte real_buffer	= m_buffer - 3;
		XRAY_FREE_IMPL	( m_allocator, real_buffer );
	}
}

inline void packet::reallocate	( u32 const new_size )
{
	m_allocated_size	= new_size;
	m_buffer_size		= std::min( m_buffer_size, m_allocated_size );
	m_buffer			= static_cast<pbyte>( XRAY_REALLOC_IMPL( m_allocator, m_buffer ? m_buffer - 3 : 0, new_size + 3, "packet" ) ) + 3;
}

inline void packet::reserve		( u32 const size )
{
	if ( m_allocated_size >= size )
		return;

	reallocate			( size );
}

inline void	packet::resize		( u32 const size )
{
	ASSERT				( !m_buffer_size );
	reserve				( size );
	m_buffer_size		= m_allocated_size;
}

inline void	packet::clone		( packet const& other )
{
	m_buffer_size		= 0;
	append				( other.m_buffer, other.m_buffer_size );
}

inline void packet::append		( u8 const value )
{
	append				( &value, sizeof(value) );
}

inline void packet::append		( s8 const value )
{
	append				( static_cast<pcvoid>( &value ), sizeof(value) );
}

inline void packet::append		( u16 const value )
{
	append				( &value, sizeof(value) );
}

inline void packet::append		( s16 const value )
{
	append				( &value, sizeof(value) );
}

inline void packet::append		( u32 const value )
{
	append				( &value, sizeof(value) );
}

inline void packet::append		( s32 const value )
{
	append				( &value, sizeof(value) );
}

inline void packet::append		( u64 const value )
{
	append				( &value, sizeof(value) );
}

inline void packet::append		( s64 const value )
{
	append				( &value, sizeof(value) );
}

inline void packet::append		( pcstr const string )
{
	append				( string, static_cast<u8>( strlen(string) ) );
}

inline void packet::append		( pcstr const string, u8 const string_length )
{
	ASSERT				( string_length <= u8(-1) );
	append				( string_length );
	append				( static_cast<pcvoid>(string), string_length );
}

inline void packet::append		( pcvoid const buffer, u32 const buffer_size )
{
	if ( m_buffer_size + buffer_size > m_allocated_size ) {
		u32 new_allocated_size	= m_allocated_size ? m_allocated_size : buffer_size;
		while ( new_allocated_size < m_buffer_size + buffer_size )
			new_allocated_size	*= 2;

		reallocate		( new_allocated_size );
	}

	ASSERT				( m_buffer_size + buffer_size <= m_allocated_size );
	memcpy				( m_buffer + m_buffer_size, buffer, buffer_size );
	m_buffer_size		+= buffer_size;
}

} // namespace network
} // namespace xray

#endif // #ifndef XRAY_NETWORK_PACKET_INLINE_H_INCLUDED