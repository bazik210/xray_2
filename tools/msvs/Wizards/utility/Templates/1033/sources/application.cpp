////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "[!output PROJECT_NAME]_application.h"
#include "[!output PROJECT_NAME]_memory.h"
#include <xray/core/core.h>
#include <xray/core/simple_engine.h>
#include <xray/os_include.h>		// for GetCommandLine

using xray::[!output PROJECT_NAME]::application;

xray::memory::doug_lea_allocator_type	xray::[!output PROJECT_NAME]::g_allocator;

typedef xray::core::simple_engine							core_engine_type;
static xray::uninitialized_reference< core_engine_type >	s_core_engine;

void application::initialize( )
{
	m_exit_code				= 0;

	XRAY_CONSTRUCT_REFERENCE( s_core_engine, core_engine_type );

	core::preinitialize		(
		&*s_core_engine,
		logging::create_log,
		GetCommandLine(), 
		command_line::contains_application_true,
		"[!output PROJECT_NAME]",
		__DATE__ 
	);

	g_allocator.do_register	( [!output MEMORY_ARENA_SIZE], "[!output PROJECT_NAME]" );

	memory::allocate_region	( );

	core::initialize		( "../../resources/sources", "[!output PROJECT_NAME]", core::perform_debug_initialization );
}

void application::finalize	( )
{
	core::finalize			( );

	XRAY_DESTROY_REFERENCE	( s_core_engine );
}

void application::execute	( )
{
	#pragma message( XRAY_TODO("place your code here") )

	m_exit_code				= s_core_engine->get_exit_code();
}