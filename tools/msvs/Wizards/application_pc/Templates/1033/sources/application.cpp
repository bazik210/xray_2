////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "[!output PROJECT_NAME]_application.h"
#include <xray/engine/api.h>
[!output OS_INCLUDE]
using [!output PROJECT_NAME]::application;

void application::initialize( )
{
	m_exit_code						= 0;

[!output PRE_INITIALIZE]
	xray::engine::initialize		( );[!output POST_INITIALIZE]
}

void application::finalize	( )
{
	xray::engine::finalize			( );
}

void application::execute	( )
{
	xray::engine::execute			( );
	m_exit_code						= xray::engine::get_exit_code();
}