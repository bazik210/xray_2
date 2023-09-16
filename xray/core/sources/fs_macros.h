////////////////////////////////////////////////////////////////////////////
//	Created		: 06.05.2010
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef FS_MACROS_H_INCLUDED
#define FS_MACROS_H_INCLUDED

#include <xray/memory_synchronized_allocator.h>

namespace xray {
namespace memory {

#if !XRAY_USE_CRT_MEMORY_ALLOCATOR
typedef memory::memory_synchronized_allocator<memory::doug_lea_allocator_type>	fs_allocator;
//typedef memory::doug_lea_allocator_type	fs_allocator;
#else
	typedef memory::crt_allocator_type	fs_allocator;
#endif

extern	fs_allocator				g_fs_allocator;

} // namespace memory
} // namespace xray

#define FS_NEW( type )			XRAY_NEW_IMPL(		::xray::memory::g_fs_allocator, type )
#define FS_DELETE( pointer )	XRAY_DELETE_IMPL(	::xray::memory::g_fs_allocator, pointer )
#define FS_ALLOC( type, count )	XRAY_ALLOC_IMPL(	::xray::memory::g_fs_allocator, type, count )
#define FS_FREE( pointer )		XRAY_FREE_IMPL(		::xray::memory::g_fs_allocator, pointer )

#endif // #ifndef FS_MACROS_H_INCLUDED