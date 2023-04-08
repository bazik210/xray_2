////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef FUNCTOR_RESPONSE_H_INCLUDED
#define FUNCTOR_RESPONSE_H_INCLUDED

#include "response.h"

namespace xray {
namespace network {

class functor_response : public response {
public:
	typedef boost::function< void ( ) >	functor_type;

public:
	inline			functor_response( functor_type const& functor ) : m_functor( functor ) { }
	virtual	void	execute			( ) { m_functor( ); }

private:
	functor_type	m_functor;
}; // class functor_response

} // namespace network
} // namespace xray

#endif // #ifndef FUNCTOR_RESPONSE_H_INCLUDED