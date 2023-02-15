////////////////////////////////////////////////////////////////////////////
//	Created		: 25.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define _WIN32_WINNT	0x0501
#include <boost/asio.hpp>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <winsock2.h>
#include <mysql/mysql.h>

typedef unsigned int	u32;
typedef unsigned short	u16;
typedef unsigned char	u8;

#include <xray/login_server/message_types.h>

#endif // #ifndef PCH_H_INCLUDED