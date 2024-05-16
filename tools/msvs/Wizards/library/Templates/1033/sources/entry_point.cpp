////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "[!output PROJECT_NAME]_world.h"
#include <xray/[!output PROJECT_NAME]/api.h>

using xray::[!output PROJECT_NAME]::[!output PROJECT_NAME]_world;

static xray::uninitialized_reference<[!output PROJECT_NAME]_world>	s_world;
xray::[!output PROJECT_NAME]::allocator_type*	xray::[!output PROJECT_NAME]::g_allocator = 0;

xray::[!output PROJECT_NAME]::world* xray::[!output PROJECT_NAME]::create_world	( engine& engine )
{
	XRAY_CONSTRUCT_REFERENCE	( s_world, [!output PROJECT_NAME]_world )	( engine );
	return						( &*s_world );
}

void xray::[!output PROJECT_NAME]::destroy_world					( xray::[!output PROJECT_NAME]::world*& world )
{
	R_ASSERT					( &*s_world == world );
	XRAY_DESTROY_REFERENCE		( s_world );
	world						= 0;
}

void xray::[!output PROJECT_NAME]::memory_allocator				( allocator_type& allocator )
{
	ASSERT						( !g_allocator );
	g_allocator					= &allocator;
}