////////////////////////////////////////////////////////////////////////////
//	Created		: 27.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_SERVER_MEMORY_H_INCLUDED
#define GAME_SERVER_MEMORY_H_INCLUDED

namespace xray {
namespace game_server {

extern	xray::memory::doug_lea_allocator_type	g_allocator;

#define USER_ALLOCATOR							g_allocator
#include <xray/std_containers.h>
#undef USER_ALLOCATOR

} // namespace game_server
} // namespace xray

#define NEW( type )								XRAY_NEW_IMPL(		::xray::game_server::g_allocator, type )
#define DELETE( pointer )						XRAY_DELETE_IMPL(	::xray::game_server::g_allocator, pointer )
#define MALLOC( size, description )				XRAY_MALLOC_IMPL(	::xray::game_server::g_allocator, size, description )
#define REALLOC( pointer, size, description )	XRAY_REALLOC_IMPL(	::xray::game_server::g_allocator, pointer, size, description )
#define FREE( pointer )							XRAY_FREE_IMPL(		::xray::game_server::g_allocator, pointer )
#define ALLOC( type, count )					XRAY_ALLOC_IMPL(	::xray::game_server::g_allocator, type, count )

#endif // #ifndef GAME_SERVER_MEMORY_H_INCLUDED