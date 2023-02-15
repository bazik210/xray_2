////////////////////////////////////////////////////////////////////////////
//	Created		: 25.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#ifdef WIN32
#	define _WIN32_WINNT	0x0501
#endif // #ifdef WIN32

#ifdef _MSC_VER
#	define _CRT_SECURE_NO_WARNINGS
#endif // 

//#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//#endif // #ifdef _DEBUG

#include <boost/asio.hpp>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <winsock2.h>
#include <mysql/mysql.h>

typedef unsigned long long	u64;
typedef unsigned int		u32;
typedef unsigned short		u16;
typedef unsigned char		u8;
typedef u8*					pbyte;
typedef char*				pstr;
typedef char const*			pcstr;

#include <xray/login_server/message_types.h>

#ifndef NDEBUG
	namespace xray {
		namespace debug {
			template < typename T >
			inline T const& identity	( T const& value )
			{
				return	value;
			}
		} // namespace debug
	} // namespace xray

#	define ASSERT( expression )	\
		if ( !xray::debug::identity(expression) ) \
			__debugbreak	( ); \
		else (void)0
#else // #ifndef NDEBUG
#	define ASSERT( ... )	(void)0
#endif // #ifndef NDEBUG

#ifndef NDEBUG
#	define XRAY_DEBUG_LOGGING_ENABLED
#endif // #ifndef NDEBUG

#ifdef XRAY_DEBUG_LOGGING_ENABLED
#	define	LOG				printf
#else // #ifdef XRAY_DEBUG_LOGGING_ENABLED
#	define	LOG(...)		(void)0
#endif // #ifdef XRAY_DEBUG_LOGGING_ENABLED

#define XRAY_ERROR_LOGGING_ENABLED

#ifdef XRAY_ERROR_LOGGING_ENABLED
#	define	LOG_ERROR		printf
#else // #ifdef XRAY_DEBUG_LOGGING_ENABLED
#	define	LOG_ERROR(...)	(void)0
#endif // #ifdef XRAY_DEBUG_LOGGING_ENABLED

typedef	u32					digest_type[ 5 ];

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

#endif // #ifndef PCH_H_INCLUDED