////////////////////////////////////////////////////////////////////////////
//	Created		: 25.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <locale.h>
#include "login_server.h"
#include <xray/login_server/constants.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <conio.h>
#include <boost/thread.hpp>

#ifdef _DEBUG
#pragma init_seg( compiler )

u32 s_requests_count	= 0;

int YourAllocHook( int allocType, void *userData, size_t size, int 
blockType, long requestNumber, const unsigned char *filename, int 
lineNumber)
{
	switch ( allocType  ) {
		case _HOOK_ALLOC : {
			++s_requests_count;
			break;
		}
		case _HOOK_REALLOC : {
			if ( size)
				++s_requests_count;
			break;
		}
		case _HOOK_FREE : {
			break;
		}
	}
	return	TRUE;
}

struct hook_initializer {
	hook_initializer()
	{
		_CrtSetAllocHook( &YourAllocHook );
	}
}; // struct hook_initializer

static hook_initializer s_hook_initializer;
#endif // #ifdef _DEBUG

namespace xray {
	u32	sign_in_tolerant_attempts_count;
	u32	sign_in_tolerant_wait_time;
	u32	sign_in_intolerant_wait_time;
} // namespace xray

static bool setup_program_options	( int argc, char* argv[], boost::program_options::variables_map& options )
{
	namespace po = boost::program_options;

	po::options_description misc_options( "Miscellanious options" );
	misc_options.add_options()
		( "help",																	"produce help message" )
		( "threads_count",			po::value< u32 >()->default_value( boost::thread::hardware_concurrency() ? boost::thread::hardware_concurrency() : 1 ),	"set how many threads may server spawn to accept connections and process asynchronous callbacks" )
		( "max_connections_count",	po::value< u32 >()->default_value( 1 << 14 ),	"set how many simultaneous connections are possible" )
	;

	po::options_description database_options( "Database options" );
	database_options.add_options()
		( "database_server",	po::value< std::string >()->default_value( "localhost" ),	"set database server name"		)
		( "database_name",		po::value< std::string >()->default_value( "stalker" ),		"set database to operate on"	)
		( "database_user",		po::value< std::string >()->default_value( "root" ),		"set database user"				)
		( "database_password",	po::value< std::string >()->default_value( "123" ),			"set database password"			)
	;

	po::options_description sign_in_options( "Sign in options" );
	sign_in_options.add_options()
		( "tolerant_attempts_count",	po::value<u32>( &xray::sign_in_tolerant_attempts_count )->default_value( 3 ),	"set tolerant attempts count" )
		( "tolerant_wait_time",			po::value<u32>( &xray::sign_in_tolerant_wait_time )->default_value( 1 ),		"set tolerant wait interval for sign in attempt, in seconds" )
		( "intolerant_wait_time",		po::value<u32>( &xray::sign_in_intolerant_wait_time )->default_value( 10 ),		"set intolerant wait interval for sign in attempt, in seconds" )
	;

	po::options_description server_options( "Server options" );
	server_options.add_options()
		( "collect_interval",			po::value<u32>()->default_value( 60 ),		"set time interval for collecting silently disconnected users, in seconds" )
		( "collect_no_ping_period",		po::value<u32>()->default_value( 10*60 ),	"set how many time user may keep silence without being disconnected, in seconds" )
		( "collect_on_start_up",		po::value<bool>()->default_value( true ),	"set if server should collect garbage on startup" )
		( "reuse_client_sessions",		po::value<bool>()->default_value( true ),	"set if server should reuse client sessions" );
	;

	po::options_description ssl_options( "SSL options" );
	ssl_options.add_options()
		( "certification_chain_file",	po::value<std::string>()->default_value( "../../resources/ssl/stalker_login_server.crt" ),		"set chain certification file" )
		( "private_key_file",			po::value<std::string>()->default_value( "../../resources/ssl/stalker_login_server.key" ),		"set private key file" )
		( "tmp_dh_file",				po::value<std::string>()->default_value( "../../resources/ssl/stalker_login_server.dh512" ),	"set temporary DH (Diffie-Hellman) parameter file" )
		( "reuse_ssl_sessions",			po::value<bool>()->default_value( true ),	"set if server should reuse SSL sessions" );
	;

	po::options_description command_line_options( "Options" );
	command_line_options.add( misc_options ).add( database_options ).add( sign_in_options ).add(server_options ).add( ssl_options );

	try {
		po::store			(
			po::command_line_parser( argc, argv )
				.options( command_line_options )
				.style( po::command_line_style::default_style | po::command_line_style::allow_long_disguise )
				.run(),
			options
		);
		po::notify			( options );    
	}
	catch ( std::exception const& exception ) {
		(void)exception;
		LOG					( "%s, exiting\r\n", exception.what() );
		return				true;
	}

	if ( options.count("help") ) {
		command_line_options.print	( std::cout );
		return				true;
	}

	return					false;
}

static void run_server				( boost::program_options::variables_map const& options )
{
	try	{
		boost::asio::io_service io_service;
		{
			xray::login_server server( io_service, xray::login_tcp_port, options );
			io_service.run		( );
		}
	}
	catch ( std::exception const& exception ) {
		(void)exception;
		LOG					( "exception: %s\r\n", exception.what() );
	}
}

int main							( int argc, char* argv[] )
{
	setlocale				( LC_ALL, "" );

	LOG						( "S.T.A.L.K.E.R. World - Login Server\r\n\r\n" );

	boost::program_options::variables_map options;
	if ( setup_program_options( argc, argv, options ) )
		return				0;

	// for MySQL to be thread safe!
	mysql_library_init		( 0, 0, 0 );

	u32 const thread_pool_size	= options["threads_count"].as<u32>( );
	boost::thread_group threads;
	for (u32 i = 0; i < thread_pool_size; ++i) {
		threads.create_thread	(
			boost::bind(
				&run_server,
				boost::cref( options )
			)
		);
	}

	threads.join_all		( );

#ifdef _DEBUG
	_CrtDumpMemoryLeaks		( );
#endif // #ifdef _DEBUG
	return					0;
}