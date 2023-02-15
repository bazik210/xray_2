////////////////////////////////////////////////////////////////////////////
//	Created		: 27.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "login_client.h"
#include "lobby_client.h"
#include "match_client.h"

namespace xray {

struct connection_info;

class client : private boost::noncopyable {
public:
					client							( boost::asio::io_service& io_service, pcstr login_host );
					~client							( );
			void	sign_in							( pcstr account_name, pcstr password );
			void	sign_out						( );
			void	sign_up							( sign_up_info const& sign_up_info, bool& finished );

public:
	inline	bool	is_signed_in					( ) const { return m_is_signed_in; }
	inline	bool	has_last_command_failed			( ) const { return m_error_description != 0; }
	inline	pcstr	last_command_error_description	( ) const { ASSERT(m_error_description); return m_error_description; }

public:
	enum status_enum {
		not_ready_for_battle,
		ready_for_battle,
	}; // enum status_enum
			void	set_status						( status_enum const status );

private:
			void	update_signed_in_state			( );
			void	on_lobby_connected				( lobby_client_message_types_enum const message_type );
			void	on_signed_in					(
						connection_error_types_enum connection_result,
						handshaking_error_types_enum handshake_error,
						socket_error_types_enum socket_error,
						login_client_message_types_enum message_type,
						connection_info const& connection_info
					);
			void	on_signed_out					(
						connection_error_types_enum connection_result,
						handshaking_error_types_enum handshake_error,
						socket_error_types_enum socket_error,
						login_client_message_types_enum message_type
					);
			void	on_signed_up					(
						connection_error_types_enum const connection_error,
						handshaking_error_types_enum const handshaking_error,
						socket_error_types_enum const socket_error,
						login_client_message_types_enum const message_type,
						sign_up_info const& sign_up_info,
						bool& finished
					);

private:
	login_client				m_login_client;
	lobby_client				m_lobby_client;
	match_client				m_match_client;
	boost::asio::io_service&	m_io_service;
	pcstr						m_error_description;
	bool						m_is_signed_in;
}; // class client

} // namespace xray

#endif // #ifndef CLIENT_H_INCLUDED