////////////////////////////////////////////////////////////////////////////
//	Created		: 27.01.2012
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
#endif // #ifdef _MSC_VER

#include <boost/asio.hpp>

#include <boost/function.hpp>
#include <boost/bind.hpp>

typedef unsigned long long	u64;
typedef unsigned int		u32;
typedef unsigned short		u16;
typedef unsigned char		u8;
typedef char const*			pcstr;
typedef void const*			pcvoid;
typedef void*				pvoid;
typedef u8 const*			pcbyte;
typedef u8*					pbyte;

#include <xray/login_server/message_types.h>
#include <xray/login_server/constants.h>

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

#if 0//ndef NDEBUG
#	define XRAY_DEBUG_LOGGING_ENABLED
#endif // #ifndef NDEBUG

#ifdef XRAY_DEBUG_LOGGING_ENABLED
#	define	LOG				printf
#else // #ifdef XRAY_DEBUG_LOGGING_ENABLED
#	define	LOG(...)		(void)0
#endif // #ifdef XRAY_DEBUG_LOGGING_ENABLED

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

#endif // #ifndef PCH_H_INCLUDED