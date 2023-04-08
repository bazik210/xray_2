////////////////////////////////////////////////////////////////////////////
//	Created		: 25.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "lobby_server.h"
#include "client_session.h"

using xray::lobby_server;
using boost::asio::ip::tcp;

lobby_server::lobby_server		( boost::asio::io_service& io_service, short const port ) :
	io_service_					( io_service ),
	acceptor_					( io_service, tcp::endpoint(tcp::v4(), port) )
{
	mysql_init					( &connection_ );
	if ( !mysql_real_connect( &connection_, "localhost", "root", "123", "stalker", 0, NULL, 0) ) {
		printf					( "can't connect to stalker database, exiting...\r\n" );
		return;
	}

	printf						( "connected to database successfully\r\n" );
	start_accept				( );
}

lobby_server::~lobby_server		( )
{
	mysql_close					( &connection_ );
}

void lobby_server::start_accept	( )
{
	printf						( "waiting for new client...\r\n" );
	client_session* const new_client	= new client_session( io_service_, connection_ );
	acceptor_.async_accept		(
		new_client->socket(),
		boost::bind(
			&lobby_server::handle_accept,
			this,
			new_client,
			boost::asio::placeholders::error
		)
	);
}

void lobby_server::handle_accept(
		xray::client_session* const new_client,
		boost::system::error_code const& error
	)
{
	if ( !error )
	  new_client->start			( );
	else
	  delete					new_client;

	start_accept				( );
}