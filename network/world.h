////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_NETWORK_WORLD_H_INCLUDED
#define XRAY_NETWORK_WORLD_H_INCLUDED

namespace xray {
namespace network {

struct XRAY_NOVTABLE world {
	virtual	void	initialize			( ) = 0;
	virtual	void	finalize			( ) = 0;
	virtual	void	tick				( ) = 0;
	virtual	void	clear_resources		( ) = 0;
	virtual	void	dispatch_callbacks	( ) = 0;

protected:
	XRAY_DECLARE_PURE_VIRTUAL_DESTRUCTOR( world );
}; // class world

} // namespace network
} // namespace xray

#endif // #ifndef XRAY_NETWORK_WORLD_H_INCLUDED