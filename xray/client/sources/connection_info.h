////////////////////////////////////////////////////////////////////////////
//	Created		: 31.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef CONNECTION_INFO_H_INCLUDED
#define CONNECTION_INFO_H_INCLUDED

namespace xray {

struct lobby_connection_info {
	u32		session_id;
	char	host[max_host_name_length];
}; // struct lobby_connection_info

struct connection_info {
//	security_connection_info	security;
	lobby_connection_info		lobby;
//	chat_connection_info		chat;
}; // struct connection_info

} // namespace xray

#endif // #ifndef CONNECTION_INFO_H_INCLUDED