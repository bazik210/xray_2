////////////////////////////////////////////////////////////////////////////
//	Created		: 27.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef CONNECT_ORDER_H_INCLUDED
#define CONNECT_ORDER_H_INCLUDED

namespace xray {
namespace network {

class connect_order :
	public order,
	private boost::noncopyable
{
public:
	typedef boost::function< void ( pcstr host, u16 port ) >	functor_type;

public:
	inline			connect_order	( functor_type const& functor, pcstr const host, u16 const port, memory::base_allocator& allocator ) :
		m_functor	( functor ),
		m_host		( strings::duplicate(allocator, host) ),
		m_port		( port ),
		m_allocator	( allocator )
	{
	}

	virtual			~connect_order	( ) { pstr temp = m_host; XRAY_FREE_IMPL( m_allocator, temp ); }
	virtual	void	execute			( ) { m_functor( m_host, m_port ); }

private:
	functor_type const		m_functor;
	memory::base_allocator&	m_allocator;
	pstr const				m_host;
	u16 const				m_port;
}; // class connect_order

} // namespace network
} // namespace xray

#endif // #ifndef CONNECT_ORDER_H_INCLUDED