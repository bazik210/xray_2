////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "network_world.h"
#include <xray/network/api.h>

using xray::network::network_world;

static xray::uninitialized_reference<network_world>	s_world;
xray::network::allocator_type*	xray::network::g_allocator = 0;

xray::network::world* xray::network::create_world	( engine& engine, xray::memory::base_allocator& orders_allocator )
{
	XRAY_CONSTRUCT_REFERENCE	( s_world, network_world )	( engine, orders_allocator );
	return						( &*s_world );
}

void xray::network::destroy_world					( xray::network::world*& world )
{
	R_ASSERT					( &*s_world == world );
	XRAY_DESTROY_REFERENCE		( s_world );
	world						= 0;
}

void xray::network::memory_allocator				( allocator_type& allocator )
{
	ASSERT						( !g_allocator );
	g_allocator					= &allocator;
}