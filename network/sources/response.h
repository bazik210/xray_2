////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef RESPONSE_H_INCLUDED
#define RESPONSE_H_INCLUDED

namespace xray {
namespace network {

class response {
public:
	virtual			~response	( ) { }
	virtual	void	execute		( ) = 0;

public:
	response*		next_for_responses;
}; // class response

} // namespace network
} // namespace xray

#endif // #ifndef RESPONSE_H_INCLUDED