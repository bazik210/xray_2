////////////////////////////////////////////////////////////////////////////
//	Created		: 07.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#if XRAY_LOGIN_SERVER_USES_SSL
#	pragma comment( lib, "libeay32-vc90-mt-s.lib" )
#	pragma comment( lib, "ssleay32-vc90-mt-s.lib" )
#endif // #if XRAY_LOGIN_SERVER_USES_SSL