////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "[!output PROJECT_NAME]_engine.h"
#include <xray/fs_path.h>
#include <xray/fs_utils.h>

using xray::[!output PROJECT_NAME]::[!output PROJECT_NAME]_engine;

[!output PROJECT_NAME]_engine::[!output PROJECT_NAME]_engine				( ) 
{
	assign_resource_path( );
}

static void module_path_dir				( string_path dir_path )
{
	pcstr const module_name	= "xray.[!output PROJECT_NAME].mll";
	HMODULE const handle	= GetModuleHandle( module_name  );
	ASSERT					( handle );
	
	GetModuleFileName		( handle, dir_path, sizeof( string_path ) );

	xray::fs::path_string	path_string	= xray::fs::convert_to_portable( dir_path );
	xray::fs::path_string	dir_path_string;
	xray::fs::directory_part_from_path ( &dir_path_string, path_string.c_str() );

	xray::strings::copy		( dir_path, sizeof( string_path ), dir_path_string.c_str() );
}

void [!output PROJECT_NAME]_engine::assign_resource_path	( )
{
	module_path_dir			( m_resource_path );
	xray::strings::join		( m_resource_path, sizeof( m_resource_path ), m_resource_path, "/../../resources/" );
	module_path_dir			( m_underscore_G_path );
	xray::strings::join		( m_underscore_G_path, sizeof( m_underscore_G_path ), m_underscore_G_path, "/../../resources/scripts/" );
}

pcstr [!output PROJECT_NAME]_engine::get_resource_path	( )const			
{ 
	return					m_resource_path; 
}

pcstr [!output PROJECT_NAME]_engine::get_underscore_G_path( )const
{ 
	return					m_underscore_G_path; 
}