////////////////////////////////////////////////////////////////////////////
//	Created		: 27.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef LOGIN_CLIENT_H_INCLUDED
#define LOGIN_CLIENT_H_INCLUDED

#if XRAY_LOGIN_SERVER_USES_SSL
#	include <boost/asio/ssl.hpp>
#endif // #if XRAY_LOGIN_SERVER_USES_SSL

namespace xray {

struct sign_up_info;
struct connection_info;

class login_client : private boost::noncopyable {
public:
	typedef boost::function<
		void (
			connection_error_types_enum,
			handshaking_error_types_enum,
			socket_error_types_enum,
			login_client_message_types_enum,
			connection_info const&
		) >			sign_in_callback_type;
	typedef boost::function<
		void (
			connection_error_types_enum,
			handshaking_error_types_enum,
			socket_error_types_enum,
			login_client_message_types_enum
		) >			sign_out_callback_type;
	typedef boost::function<
		void (
			connection_error_types_enum,
			handshaking_error_types_enum,
			socket_error_types_enum,
			login_client_message_types_enum,
			sign_up_info const&
		) >			sign_up_callback_type;

public:
					login_client				( boost::asio::io_service& io_service, pcstr host );
					~login_client				( );
			void	sign_up						( sign_up_info const& sign_up_info, sign_up_callback_type const& callback );
			void	sign_in						( pcstr account_name, pcstr password, sign_in_callback_type const& callback );
			void	sign_out					( sign_out_callback_type const& callback );
	inline	bool	is_signed_in				( ) const { return m_client_state == signed_in; }
	inline	bool	is_signed_out				( ) const { return m_client_state == signed_out; }

//	SSL
private:
#if XRAY_LOGIN_SERVER_USES_SSL
			bool	verify_ssl_certificate		( bool const preverified, boost::asio::ssl::verify_context& verify_context );
#endif // #if XRAY_LOGIN_SERVER_USES_SSL

// resolve
private:
	typedef boost::function< void ( resolve_error_types_enum, tcp::resolver::iterator ) >	on_resolved_functor_type;

			void	on_resolved					(
						tcp::resolver* const resolver,
						u32 const retry_count,
						on_resolved_functor_type const& functor,
						boost::system::error_code const& error_code,
						tcp::resolver::iterator iterator
					);
			void	resolve						( on_resolved_functor_type const& functor, u32 const retry_count );

// connect
private:
	typedef boost::function< void ( connection_error_types_enum ) >		on_connected_functor_type;
			void	on_connected				(
						u32 const retry_count,
						on_connected_functor_type const& functor,
						boost::system::error_code const& error_code,
						tcp::resolver::iterator iterator
					);
			void	connect						(
						resolve_error_types_enum error,
						tcp::resolver::iterator iterator,
						u32 const retry_count,
						on_connected_functor_type const& functor
					);

// handshake
private:
	typedef boost::function< void ( handshaking_error_types_enum ) >	on_handshaked_functor_type;
			void	on_handshake_connected		(
						xray::connection_error_types_enum const connection_result,
						on_handshaked_functor_type const& functor,
						u32 const retry_count
					);
			void	on_handshaked				(
						boost::system::error_code const& error_code,
						on_handshaked_functor_type const& functor,
						u32 retry_count
					);
			void	handshake					( on_handshaked_functor_type const& functor, u32 const retry_count );

// disconnect
private:
			void	establish_connection		( on_connected_functor_type const& functor, u32 const resolve_retry_count, u32 const reconnect_retry_count );
			void	close_connection			( );

// sign in
private:
			void	on_sign_in_answer_received	(
						sign_in_callback_type const& callback,
						boost::system::error_code const& error_code,
						size_t const bytes_transferred
					);
			void	on_sign_in_password_written	(
						sign_in_callback_type const& callback,
						boost::system::error_code const& error_code,
						size_t const bytes_transferred
					);
			void	on_sign_in_handshaked		( sign_in_callback_type const& callback, handshaking_error_types_enum const error );
			void	on_sign_in_written			( sign_in_callback_type const& callback, boost::system::error_code const& error_code, size_t bytes_transferred );
			void	sign_in_on_connected		( connection_error_types_enum connection_result, sign_in_callback_type const& callback );

// sign out
private:
			void	on_sign_out_password_written(
						sign_out_callback_type const& callback,
						boost::system::error_code const& error_code,
						size_t const bytes_transferred
					);
			void	on_sign_out_handshaked		( sign_out_callback_type const& callback, handshaking_error_types_enum const error );
			void	on_sign_out_written			( sign_out_callback_type const& callback, boost::system::error_code const& error_code, size_t bytes_transferred );
			void	sign_out_on_connected		( connection_error_types_enum connection_result, sign_out_callback_type const& callback );

// sign up
private:
			void	on_sign_up_answer_received	(
						sign_up_callback_type const& callback, 
						xray::sign_up_info const& sign_up_info,
						boost::system::error_code const& error_code,
						size_t const bytes_transferred
					);
			void	on_sign_up_info_written		(
						sign_up_callback_type const& callback,
						xray::sign_up_info const& sign_up_info,
						boost::system::error_code const& error_code,
						size_t const bytes_transferred
					);
			void	sign_up_on_handshaked		(
						sign_up_callback_type const& callback,
						xray::sign_up_info const& sign_up_info,
						xray::handshaking_error_types_enum const handshaking_result
					);
			void	on_sign_up_account_answer_received	(
						sign_up_callback_type const& callback,
						xray::sign_up_info const& sign_up_info,
						boost::system::error_code const& error_code,
						size_t const bytes_transferred
					);
			void	on_sign_up_written			(
						sign_up_callback_type const& callback,
						xray::sign_up_info const& sign_up_info,
						boost::system::error_code const& error_code,
						size_t const bytes_transferred
					);
			void	sign_up_on_connected		(
						xray::connection_error_types_enum const connection_result,
						sign_up_callback_type const& callback,
						xray::sign_up_info const& sign_up_info
					);

// ping functionality
private:
			enum {
				ping_retry_count				= 10,
			};
			void	on_ping_sent				(
						u32 const try_count,
						boost::system::error_code const& error_code,
						size_t const bytes_transferred
					);
			void	ping						( u32 retry_count );

private:
	enum client_state_enum {
		signing_out,
		signed_out,
		signing_in,
		signed_in,
		signing_up,
	}; // enum client_state_enum

private:
	enum connection_state_enum {
		unresolved,
		resolving,
		resolved,
		connecting,
		connected,
		handshaking,
		handshaked,
	}; // enum connection_state_enum

private:
#if XRAY_LOGIN_SERVER_USES_SSL
	typedef boost::asio::ssl::stream< tcp::socket& >	ssl_stream_type;
#else // #if XRAY_LOGIN_SERVER_USES_SSL
	typedef tcp::socket&								ssl_stream_type;
#endif // #if XRAY_LOGIN_SERVER_USES_SSL
	// it is a pity, but the next 3 members ordering is important,
	// since all of them are initialized in constructor
	// and SSL stream uses both socket and SSL context
	tcp::socket					m_socket;
#if XRAY_LOGIN_SERVER_USES_SSL
	boost::asio::ssl::context	m_ssl_context;
#endif // #if XRAY_LOGIN_SERVER_USES_SSL
	ssl_stream_type				m_ssl_stream;

	udp::socket					m_ping_socket;
	boost::asio::deadline_timer	m_ping_timer;
	boost::asio::io_service&	m_io_service;
	client_state_enum			m_client_state;
	connection_state_enum		m_connection_state;
	u32							m_session_id;
	char						m_host[ max_host_name_length ];
	char						m_password[ max_password_length ];
	char						m_account_name[ max_account_name_length ];
	u8							m_data[ 1 + (1 + max_account_name_length) + (1 + max_email_length) ];
}; // class login_client

} // namespace xray

#endif // #ifndef ENGINE_LOGIN_CLIENT_LOGIN_CLIENT_H_INCLUDED