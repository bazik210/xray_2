////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "[!output PROJECT_NAME]_application.h"
#include <xray/engine/api.h>
#include <xray/os_include.h>

using [!output PROJECT_NAME]::application;

void application::initialize( )
{
	m_exit_code					= 0;

	xray::engine::preinitialize	( m_game_proxy, GetCommandLine( ), "[!output PROJECT_NAME]", __DATE__ );
	xray::engine::initialize	( );
}

void application::finalize	( )
{
	xray::engine::finalize		( );
}

void application::execute	( )
{
	xray::engine::execute		( );
	m_exit_code					= xray::engine::get_exit_code();
}