////////////////////////////////////////////////////////////////////////////
//	Created		: 26.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "client_session.h"
#include "server_impl.h"
#include "network_world.h"

using xray::network::client_session;

client_session::client_session	(
		boost::asio::io_service& io_service,
		xray::network::server_impl& server
	) :
	m_socket		( io_service ),
	m_packet_socket	( m_socket, server.world() ),
	next			( 0 )
{
}

client_session::~client_session	( )
{
	m_packet_socket.stop_receiving	( );
}

void client_session::start		( )
{
	m_packet_socket.start_receiving	( );
}