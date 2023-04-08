////////////////////////////////////////////////////////////////////////////
//	Created		: 24.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef MESSAGE_TYPES_H_INCLUDED
#define MESSAGE_TYPES_H_INCLUDED

#define XRAY_LOGIN_SERVER_USES_SSL					1

namespace xray {

// client to server
enum login_server_message_types_enum {
	sign_up_message_type							= 0,
	sign_in_message_type							= 1,
	sign_out_message_type							= 2,

	login_server_invalid_message_type				= 63,
}; // enum login_server_message_types_enum

// server to client
enum login_client_message_types_enum {
	// sign in
	lobby_info_message_type							= 64,
	password_request_message_type					= 65,
	invalid_user_name_or_password_message_type		= 66,
	sign_in_attempt_interval_violated_message_type	= 67,

	// sign out
	sign_out_successful								= 68,

	// sign up
	occupied_user_name_message_type					= 69,
	send_sign_up_info_message_type					= 70,
	sign_up_successful_message_type					= 71,

	login_client_invalid_message_type				= 127,
}; // enum login_client_message_types_enum

// client to server
enum lobby_server_message_types_enum {
	set_status_ready_for_battle						= 128,

	lobby_server_invalid_message_type				= 191,
}; // enum lobby_server_message_types_enum

// server to client
enum lobby_client_message_types_enum {
	connection_successful							= 192,
	invalid_session_id								= 193,
	connect_to_battle_server						= 194,

	lobby_client_invalid_message_type				= 255,
}; // enum lobby_client_message_types_enum

enum socket_error_types_enum {
	no_socket_error									= 0,
	unable_to_write_to_socket						= 1,
	unable_to_read_from_socket						= 2,
	invalid_socket_error_type						= 255,
}; // enum socket_error_types_enum

enum resolve_error_types_enum {
	successfully_resolved							= 0,
	cannot_resolve									= 1,
	resolve_error_type								= 255,
}; // enum connection_error_tresolve_error_types_enumypes_enum

enum connection_error_types_enum {
	successfully_connected							= 0,
	cannot_connect									= 1,
	connection_error_type							= 255,
}; // enum connection_error_types_enum

enum handshaking_error_types_enum {
	successfully_handshaked							= 0,
	cannot_handshake								= 1,
	no_handshake 									= 2,
	handshaking_error_type							= 255,
}; // enum handshaking_error_types_enum

} // namespace xray

#endif // #ifndef MESSAGE_TYPES_H_INCLUDED