////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef SEND_ORDER_H_INCLUDED
#define SEND_ORDER_H_INCLUDED

#include "order.h"
#include <xray/network/packet.h>

namespace xray {
namespace network {

class packet;

class send_order :
	public order,
	private boost::noncopyable
{
public:
	typedef boost::function< void ( packet const& ) >	send_type;

	inline				send_order	(
			send_type const& sender,
			packet const& packet,
			memory::base_allocator& allocator
		) :
		m_sender			( sender ),
		m_packet			( packet ),
		m_allocator			( allocator )
	{
	}

	virtual				~send_order	( )
	{
		packet const* temp	= &m_packet;
		XRAY_DELETE_IMPL	( m_allocator, temp );
	}

	virtual	void		execute		( )
	{
		m_sender			( m_packet );
	}

private:
	send_type				m_sender;
	packet const&			m_packet;
	memory::base_allocator&	m_allocator;
}; // send_order

} // namespace network
} // namespace xray

#endif // #ifndef SEND_ORDER_H_INCLUDED