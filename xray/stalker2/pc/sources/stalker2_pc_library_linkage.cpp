////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#ifdef OPENGL
#	include <xray/engine/pc/opengl/library_linkage.h>
#else // #ifdef OPENGL
#	include <xray/engine/pc/dx11/library_linkage.h>
#endif // #ifdef OPENGL

#include <xray/game/library_linkage.h>

#ifdef XRAY_STATIC_LIBRARIES
#	include <xray/engine/library_linkage.h>
#	include <xray/engine/library_dependencies.h>

#	include <ogg/library_linkage.h>
#	include <vorbis/library_linkage.h>
#	pragma comment( lib, "x3daudio.lib" )
#	include <ode/library_linkage.h>
#	include <xray/render/engine/library_linkage.h>

#	include <xray/linkage_helper.h>
	XRAY_INCLUDE_TO_LINKAGE(sound_library_linkage)
#endif // #ifdef XRAY_STATIC_LIBRARIES