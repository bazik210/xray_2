////////////////////////////////////////////////////////////////////////////
//	Created		: 25.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef CLIENT_SESSION_H_INCLUDED
#define CLIENT_SESSION_H_INCLUDED

#include <xray/login_server/constants.h>
#include "memory_allocator.h"

#if XRAY_LOGIN_SERVER_USES_SSL
#	include <boost/asio/ssl.hpp>
#endif // #if XRAY_LOGIN_SERVER_USES_SSL

namespace boost {
	namespace asio {
		class io_service;
	} // namespace asio

	namespace system {
		class error_code;
	} // namespace system
} // namespace boost

namespace xray {

struct sign_up_info;
class login_server;

class client_session : private boost::noncopyable {
public:
#if XRAY_LOGIN_SERVER_USES_SSL
	typedef boost::asio::ssl::stream< tcp::socket& >	ssl_stream_type;
#else // #if XRAY_LOGIN_SERVER_USES_SSL
	typedef tcp::socket&								ssl_stream_type;
#endif // #if XRAY_LOGIN_SERVER_USES_SSL

public:
						client_session					(
							boost::asio::io_service& io_service,
							MYSQL& connection,
#if XRAY_LOGIN_SERVER_USES_SSL
							boost::asio::ssl::context& ssl_context,
#endif // #if XRAY_LOGIN_SERVER_USES_SSL
							login_server& login_server
						);
			void		start							( );
	inline	tcp::socket& socket							( ) { return m_socket; }

private:
	typedef boost::function<
		void ( client_session*, boost::system::error_code const& error, size_t const bytes_transferred )
	> read_handler_type;
	
	typedef boost::function<
		void ( )
	> handshake_handler_type;

	enum security_type {
		insecure,
		secure,
	}; // enum security_type

	enum action_type {
		read_after_message_sent,
		destroy_after_message_sent,
	}; // enum action_type

private:
			void 		async_read_next_message	( );
			void		on_message_sent			(
							xray::login_client_message_types_enum const message_type,
							action_type action_type,
							boost::system::error_code const& error_code,
							size_t bytes_transferred
						);
			void 		send_message			(
							login_client_message_types_enum message_type,
							security_type write_security_type,
							action_type action_type
						);

private:
			void	disconnect_and_destroy_this	( );

			void		on_handshaked			( boost::system::error_code const& error, handshake_handler_type const& functor, u32 retry_count );
			void		handshake				( handshake_handler_type const& functor );

			void 		add_new_user			( sign_up_info& sign_up_info );
			void		on_sign_up_result_sent	(
							boost::system::error_code const& error_code,
							size_t bytes_transferred
						);
			void		on_sign_up_info_received(
							std::string const& account_name,
							boost::system::error_code const& error_code,
							size_t const bytes_transferred
						);
			void		sign_up_on_handshaked	( std::string const& account_name );
			void		on_sign_up_more_info_sent(
							std::string const& account_name_string,
							boost::system::error_code const& error_code,
							size_t const bytes_transferred
						);
			void 		process_sign_up			( );

			void		update_sign_in_stats	(
							u32 account_id,
							u32 invalid_sign_in_attempts_count,
							boost::posix_time::ptime const& next_sign_in_attempt_time
						);
			void		remove_online_user		( u32 account_id );
			u32			insert_new_online_user	( u32 account_id );
			void 		on_lobby_info_sent		(
							boost::system::error_code const& error,
							size_t const bytes_transferred
						);
			void 		send_lobby_info			( u32 session_id );
			void		add_online_user			( u32 account_id, boost::posix_time::ptime const& current_time );
			void		on_sign_in_password_received	(
							u32 const account_id,
							u32 const invalid_sign_in_attempts_count,
							u32 const seconds_to_wait,
							boost::system::error_code const& error_code,
							size_t const bytes_transferred
						);
			void		sign_in_on_handshaked	(
							u32 account_id,
							u32 invalid_sign_in_attempts_count,
							u32 seconds_to_wait
						);
			void 		process_sign_in			( );

			void		remove_online_user		( u32 account_id, pcstr password );
			void		on_sign_out_password_received	(
							u32 const account_id,
							boost::system::error_code const& error_code,
							size_t const bytes_transferred
						);
			void		sign_out_on_handshaked	( u32 const account_id );
			void		process_sign_out		( u32 account_id );

			void		process_ping			( );

			void		on_account_name_received(
							u8 const expected_bytes_count,
							boost::system::error_code const& error_code,
							size_t const bytes_transferred
						);
			void 		on_client_message		( boost::system::error_code const& error, size_t const bytes_transferred );

private:
	enum {
		max_length		= 
			1 + 
			1 + max_account_name_length +
			1 + max_password_length +
			1 + max_email_length,
	};

private:
	// it is a pity, but the next 3 members ordering is important,
	// since all of them are initialized in constructor
	// and SSL stream uses socket and SSL context
	tcp::socket			m_socket;
	ssl_stream_type		m_ssl_stream;

	handler_allocator	m_allocator;
	MYSQL&				m_connection;
	login_server&		m_login_server;
#if XRAY_LOGIN_SERVER_USES_SSL
	boost::asio::ssl::context&	m_ssl_context;
#endif // #if XRAY_LOGIN_SERVER_USES_SSL

public:
	client_session*		next;

private:
	digest_type			m_password_digest;
	u8					m_data[ max_length ];
	bool				m_handshaked;
}; // class client_session

} // namespace xray

#endif // #ifndef CLIENT_SESSION_H_INCLUDED