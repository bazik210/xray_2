////////////////////////////////////////////////////////////////////////////
//	Created		: 27.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_client.h"
#include "connection_info.h"

using xray::login_client;

login_client::login_client				(
		boost::asio::io_service& io_service,
		pcstr host
	) :
	m_socket					( io_service ),
#if XRAY_LOGIN_SERVER_USES_SSL
	m_ssl_context						( boost::asio::ssl::context::sslv23 ),
	m_ssl_stream						( m_socket , m_ssl_context ),
#else // #if XRAY_LOGIN_SERVER_USES_SSL
	m_ssl_stream						( m_socket ),
#endif // #if XRAY_LOGIN_SERVER_USES_SSL
	m_ping_socket						( io_service, udp::endpoint( udp::v4(), 0 ) ),
	m_ping_timer						( io_service, boost::posix_time::seconds(1) ),
	m_io_service						( io_service ),
	m_client_state						( signed_out ),
	m_connection_state					( unresolved ),
	m_session_id						( 0 )
{
#if XRAY_LOGIN_SERVER_USES_SSL
	m_ssl_context.load_verify_file		( "../../resources/ssl/stalker_login_server.crt" );
	m_ssl_stream.set_verify_mode		( boost::asio::ssl::verify_peer );
	m_ssl_stream.set_verify_callback	(
		boost::bind(
			&login_client::verify_ssl_certificate,
			this,
			_1,
			_2
		)
	);
#endif // #if XRAY_LOGIN_SERVER_USES_SSL

	strncpy								( m_host, host, sizeof(m_host)-1 );
	m_host[ sizeof(m_host) - 1 ]		= 0;
}

login_client::~login_client				( )
{
	do {
		switch ( m_connection_state ) {
			case unresolved : return;
			case connecting : continue;
			case connected : {
				switch ( m_client_state ) {
					case signing_out : continue;
					case signed_out : {
						close_connection( );
						break;
					}
					case signing_in : continue;
					case signed_in : {
						sign_out		( sign_out_callback_type() );
						break;
					}
				}
			}
			case handshaking : continue;
			case handshaked : {
				switch ( m_client_state ) {
					case signing_out : continue;
					case signed_out : {
						close_connection( );
						break;
					}
					case signing_in : continue;
					case signed_in : {
						sign_out		( sign_out_callback_type() );
						break;
					}
				}
			}
		}
	} while ( m_connection_state != unresolved );
}

#if XRAY_LOGIN_SERVER_USES_SSL
bool login_client::verify_ssl_certificate ( bool const preverified, boost::asio::ssl::verify_context& verify_context )
{
	// The verify callback can be used to check whether the certificate that is
	// being presented is valid for the peer. For example, RFC 2818 describes
	// the steps involved in doing this for HTTPS. Consult the OpenSSL
	// documentation for more details. Note that the callback is called once
	// for each certificate in the certificate chain, starting from the root
	// certificate authority.

	// In this example we will simply print the certificate's subject name.
	//char subject_name[256];
	//X509* cert = X509_STORE_CTX_get_current_cert(verify_context.native_handle());
	//X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
	//std::cout << "Verifying " << subject_name << "\n";

	(void)verify_context;
	return								preverified;
}
#endif // #if XRAY_LOGIN_SERVER_USES_SSL

void login_client::establish_connection	( on_connected_functor_type const& functor, u32 const resolve_retry_count, u32 const reconnect_retry_count )
{
	if ( m_connection_state == unresolved ) {
		resolve								(
			boost::bind(
				&login_client::connect,
				this,
				_1,
				_2,
				reconnect_retry_count,
				functor
			),
			resolve_retry_count
		);
		return;
	}
}

void login_client::close_connection		( )
{
	LOG								( "closed connection\r\n" );
	//if ( m_connection_state == handshaked )
	//	m_ssl_stream.shutdown		( );

#if XRAY_LOGIN_SERVER_USES_SSL
	m_ssl_stream.~ssl_stream_type	( );
	new (&m_ssl_stream) ssl_stream_type	( m_socket, m_ssl_context );
#endif // #if XRAY_LOGIN_SERVER_USES_SSL
	m_socket.shutdown				( boost::asio::socket_base::shutdown_both );
	m_socket.close					( );
	m_connection_state				= unresolved;
}