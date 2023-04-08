////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef ORDER_H_INCLUDED
#define ORDER_H_INCLUDED

namespace xray {
namespace network {

class order {
public:
	virtual			~order	( ) { }
	virtual	void	execute	( ) = 0;

public:
	order*			next_for_orders;
}; // class order

} // namespace network
} // namespace xray

#endif // #ifndef ORDER_H_INCLUDED