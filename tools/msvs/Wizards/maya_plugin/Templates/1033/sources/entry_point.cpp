////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <xray/core/core.h>
#include <xray/command_line_extensions.h>
#include "[!output PROJECT_NAME]_engine.h"

#pragma comment( lib, "delayimp.lib" )

MCallbackIdArray				callbackIds;
xray::[!output PROJECT_NAME]::allocator_type		xray::[!output PROJECT_NAME]::g_allocator;
static xray::uninitialized_reference< xray::[!output PROJECT_NAME]::[!output PROJECT_NAME]_engine >	s_core_engine;

namespace boost {
	void throw_exception		( std::exception const& exception )
	{
		XRAY_UNREFERENCED_PARAMETER	( exception );
		FATAL						( "boost::throw_exception ");
	}
} // namespace boost

XRAY_DLL_EXPORT MStatus initializePlugin( MObject obj )
{
	
	xray::core::preinitialize	(
		&*s_core_engine,
		"-fpe_disabled",
		xray::command_line::contains_application_false,
		"[!output PROJECT_NAME]",
		__DATE__
	);

	xray::[!output PROJECT_NAME]::g_allocator.do_register	( [!output MEMORY_ARENA_SIZE], "[!output PROJECT_NAME]" );
	xray::memory::allocate_region ( );
	xray::core::initialize		( "[!output PROJECT_NAME]", xray::core::no_log, xray::core::perform_debug_initialization );

	MStatus						stat;
	MFnPlugin plugin			( obj, "GSC Game World", "1.0", "Any");
	return						stat;
}

XRAY_DLL_EXPORT MStatus uninitializePlugin( MObject obj)
{
	MStatus						stat;
	MFnPlugin					plugin( obj );
	return						stat;
}