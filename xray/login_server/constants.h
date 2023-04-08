////////////////////////////////////////////////////////////////////////////
//	Created		: 24.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef ENGINE_CONSTANTS_H_INCLUDED
#define ENGINE_CONSTANTS_H_INCLUDED

namespace xray {

enum {
	max_account_name_length		= 32,
	max_password_length			= 48,
	max_email_length			= 48,
	max_host_name_length		= 64,
};

enum {
	login_tcp_port				= 25100,
	login_udp_port				= 25100,

	lobby_tcp_port				= 25101,
	
	chat_tcp_port				= 25102,

	match_tcp_port				= 25103,
	match_udp_port				= 25103,
};

enum {
	login_resolve_retry_count						= 6,
	login_connect_retry_count						= 6,
	login_handshake_retry_count						= 1,
	login_seconds_to_next_sign_in_attempt			= 1,
	login_seconds_to_next_sign_in_attempt_if_many	= 10,
};

} // namespace xray

#endif // #ifndef ENGINE_CONSTANTS_H_INCLUDED