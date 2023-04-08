////////////////////////////////////////////////////////////////////////////
//	Created		: 21.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef XARY_NETWORK_PACKET_H_INCLUDED
#define XARY_NETWORK_PACKET_H_INCLUDED

//#include <boost/asio.hpp>

namespace xray {
namespace network {

class packet : private boost::noncopyable {
public:
	inline	explicit	packet			( memory::base_allocator& allocator );
	inline				~packet			( );
	inline	u32			allocated_size	( ) const { return m_allocated_size; }
	inline	void		reserve			( u32 size );
	inline	void		resize			( u32 size );
	inline	void		clone			( packet const& other );

	inline	void		append			( u8 size );
	inline	void		append			( s8 size );
	inline	void		append			( u16 size );
	inline	void		append			( s16 size );
	inline	void		append			( u32 size );
	inline	void		append			( s32 size );
	inline	void		append			( u64 size );
	inline	void		append			( s64 size );
	inline	void		append			( pcstr string );
	inline	void		append			( pcstr string, u8 string_length );
	inline	void		append			( pcvoid buffer, u32 buffer_size );

public:
	inline	pcbyte		buffer			( ) const { return m_buffer; }
	inline	pbyte		buffer			( ) { return m_buffer; }
	inline	u32			buffer_size		( ) const { return m_buffer_size; }

private:
	inline	void		reallocate		( u32 new_size );

public:
	packet*					next;

private:
	pbyte					m_buffer;
	memory::base_allocator& m_allocator;
	u32						m_buffer_size;
	u32						m_allocated_size;
}; // class packet

} // namespace network
} // namespace xray

#include <xray/network/packet_inline.h>

#endif // #ifndef XARY_NETWORK_PACKET_H_INCLUDED