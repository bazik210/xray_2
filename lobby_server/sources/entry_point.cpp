////////////////////////////////////////////////////////////////////////////
//	Created		: 25.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "lobby_server.h"

int main					( int argc, char* argv[] )
{
	(void)argc;
	(void)argv;

	try	{
		boost::asio::io_service io_service;
		xray::lobby_server server( io_service, 5001 );
		io_service.run		( );
	}
	catch ( std::exception& e ) {
		printf				( "Exception: %s\r\n", e.what() );
	}

	return					0;
}