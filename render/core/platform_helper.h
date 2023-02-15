////////////////////////////////////////////////////////////////////////////
//	Created		: 14.12.2011
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_RENDER_CORE_PLATFORM_HELPER_H_INCLUDED
#define XRAY_RENDER_CORE_PLATFORM_HELPER_H_INCLUDED

#if XRAY_PLATFORM_WINDOWS
#	define XRAY_RENDER_CORE_INCLUDE_FOLDER	dx11
#elif XRAY_PLATFORM_XBOX_360	// #if XRAY_PLATFORM_WINDOWS
#	define XRAY_RENDER_CORE_INCLUDE_FOLDER	xbox360
#elif XRAY_PLATFORM_PS3 		// #elif XRAY_PLATFORM_WINDOWS
#	define XRAY_RENDER_CORE_INCLUDE_FOLDER	ps3
#else							// #elif XRAY_PLATFORM_PS3
#	error please define your platform!
#endif							// #if XRAY_PLATFORM_WINDOWS

#define XRAY_RENDER_CORE_FILE(...)	<xray/render/core/XRAY_RENDER_CORE_INCLUDE_FOLDER/__VA_ARGS__>

#endif // #ifndef XRAY_RENDER_CORE_PLATFORM_HELPER_H_INCLUDED