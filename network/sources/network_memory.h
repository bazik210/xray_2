////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef NETWORK_MEMORY_H_INCLUDED
#define NETWORK_MEMORY_H_INCLUDED

#include <xray/network/api.h>

namespace xray {
namespace network {

extern allocator_type*							g_allocator;

#define USER_ALLOCATOR							*::xray::network::g_allocator
#include <xray/std_containers.h>
#undef USER_ALLOCATOR

} // namespace network
} // namespace xray

#define NEW( type )								XRAY_NEW_IMPL(		*::xray::network::g_allocator, type )
#define DELETE( pointer )						XRAY_DELETE_IMPL(	*::xray::network::g_allocator, pointer )
#define MALLOC( size, description )				XRAY_MALLOC_IMPL(	*::xray::network::g_allocator, size, description )
#define REALLOC( pointer, size, description )	XRAY_REALLOC_IMPL(	*::xray::network::g_allocator, pointer, size, description )
#define FREE( pointer )							XRAY_FREE_IMPL(		*::xray::network::g_allocator, pointer )
#define ALLOC( type, count )					XRAY_ALLOC_IMPL(	*::xray::network::g_allocator, type, count )

#endif // #ifndef NETWORK_MEMORY_H_INCLUDED