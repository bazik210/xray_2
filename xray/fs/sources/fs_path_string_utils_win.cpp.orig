////////////////////////////////////////////////////////////////////////////
//	Created		: 24.02.2011
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <xray/fs/fs_native_path_string.h>
#include <xray/fs/fs_path_string_utils.h>

#include <xray/os_preinclude.h>
#	undef			NOUSER            			// All USER undefs and routines
#	undef			NOMSG             			// typedef MSG and associated routines
#include <xray/os_include.h>

#include <shlwapi.h>
#pragma comment	(lib, "shlwapi.lib")

xray::fs_new::native_path_string const &	xray::fs_new::get_current_directory ()
{
	static native_path_string 				s_current_directory;
	static bool	s_initialized			=	false;
	if ( s_initialized )
		return								s_current_directory;

	string_path								path;
	GetCurrentDirectory						(sizeof(path), path);
	s_current_directory.assign_with_conversion	(path);
	s_current_directory.rtrim				(s_current_directory.separator);
	s_initialized						=	true;
	return									s_current_directory;
}
