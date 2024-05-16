////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#ifndef [!output PROJECT_NAME_UPPER]_MEMORY_H_INCLUDED
#define [!output PROJECT_NAME_UPPER]_MEMORY_H_INCLUDED

#include <xray/[!output PROJECT_NAME]/api.h>

namespace xray {
namespace [!output PROJECT_NAME] {

extern allocator_type*							g_allocator;

#define USER_ALLOCATOR							*::xray::[!output PROJECT_NAME]::g_allocator
#include <xray/std_containers.h>
#undef USER_ALLOCATOR

} // namespace [!output PROJECT_NAME]
} // namespace xray

#define NEW( type )								XRAY_NEW_IMPL(		*::xray::[!output PROJECT_NAME]::g_allocator, type )
#define DELETE( pointer )						XRAY_DELETE_IMPL(	*::xray::[!output PROJECT_NAME]::g_allocator, pointer )
#define MALLOC( size, description )				XRAY_MALLOC_IMPL(	*::xray::[!output PROJECT_NAME]::g_allocator, size, description )
#define REALLOC( pointer, size, description )	XRAY_REALLOC_IMPL(	*::xray::[!output PROJECT_NAME]::g_allocator, pointer, size, description )
#define FREE( pointer )							XRAY_FREE_IMPL(		*::xray::[!output PROJECT_NAME]::g_allocator, pointer )
#define ALLOC( type, count )					XRAY_ALLOC_IMPL(	*::xray::[!output PROJECT_NAME]::g_allocator, type, count )

#endif // #ifndef [!output PROJECT_NAME_UPPER]_MEMORY_H_INCLUDED