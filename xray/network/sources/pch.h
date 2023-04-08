////////////////////////////////////////////////////////////////////////////
//	Created		: 23.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define XRAY_NETWORK_BUILDING

#ifndef XRAY_STATIC_LIBRARIES
#	define XRAY_ENGINE_BUILDING
#endif // #ifndef XRAY_STATIC_LIBRARIES

#define XRAY_LOG_MODULE_INITIATOR	"network"
#include <xray/extensions.h>

#include "network_memory.h"

#ifdef WIN32
#	define _WIN32_WINNT		0x0600
#endif // #ifdef WIN32

#include <xray/os_include.h>

#include <boost/asio.hpp>

#endif // #ifndef PCH_H_INCLUDED