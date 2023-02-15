////////////////////////////////////////////////////////////////////////////
//	Created		: 27.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef LOBBY_CLIENT_H_INCLUDED
#define LOBBY_CLIENT_H_INCLUDED

namespace xray {

struct lobby_connection_info;

class lobby_client {
public:
	typedef boost::function1< void, lobby_client_message_types_enum >	connect_callback_type;

public:
					lobby_client	( );
			void	connect			( lobby_connection_info const& lobby_connection_info, connect_callback_type const& callback );
	inline	bool	is_connected	( ) const { return m_is_connected; }

private:
	bool			m_is_connected;
}; // class lobby_client

} // namespace xray

#endif // #ifndef LOBBY_CLIENT_H_INCLUDED