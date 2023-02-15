////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef RECEIVE_RESPONSE_H_INCLUDED
#define RECEIVE_RESPONSE_H_INCLUDED

#include "response.h"

namespace xray {
namespace network {

class receive_response :
	public response,
	private boost::noncopyable
{
public:
	typedef boost::function< void (xray::network::packet const&) >	on_packed_received_type;

	inline			receive_response	(
			on_packed_received_type const& receiver,
			xray::memory::doug_lea_allocator& allocator,
			xray::network::packet const& packet
		) :
		m_receiver				( receiver ),
		m_allocator				( allocator ),
		m_packet				( packet )
	{
	}

	virtual			~receive_response	( )
	{
		packet const* temp		= &m_packet;
		XRAY_DELETE_IMPL		( m_allocator, temp );
	}

	virtual	void	execute				( )
	{
		m_receiver				( m_packet );
	}

private:
	on_packed_received_type		m_receiver;
	memory::doug_lea_allocator&	m_allocator;
	packet const&				m_packet;
}; // receive_response

} // namespace network
} // namespace xray

#endif // #ifndef RECEIVE_RESPONSE_H_INCLUDED