////////////////////////////////////////////////////////////////////////////
//	Created		: 27.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "lobby_client.h"

using xray::lobby_client;

lobby_client::lobby_client	( ) :
	m_is_connected	( false )
{
}

void lobby_client::connect	(
		xray::lobby_connection_info const& lobby_connection_info,
		connect_callback_type const& callback
	)
{
}