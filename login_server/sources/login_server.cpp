////////////////////////////////////////////////////////////////////////////
//	Created		: 25.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "login_server.h"
#include "client_session.h"

using xray::login_server;

static long s_active_connections	= 0;

void login_server::process_client				(
		xray::client_session* const new_client,
		boost::system::error_code const& error
	)
{
	if ( !error ) {
		BOOST_INTERLOCKED_INCREMENT	( &s_active_connections );
		new_client->start			( );
	}
	else
		delete_client_session		( new_client );

	LOG								( "new client connected\r\n" );
	start_client_acception			( );
}

xray::client_session* login_server::new_client_session	( )
{
	if ( !m_client_sessions )
#if XRAY_LOGIN_SERVER_USES_SSL
		return						new client_session( m_io_service, m_connection, m_ssl_context, *this );
#else // #if XRAY_LOGIN_SERVER_USES_SSL
		return						new client_session( m_io_service, m_connection, *this );
#endif // #if XRAY_LOGIN_SERVER_USES_SSL

	client_session* const result	= m_client_sessions;
	m_client_sessions				= m_client_sessions->next;
	return							result;
}

void login_server::delete_client_session		( xray::client_session* const client_session )
{
	if ( m_reuse_client_sessions ) {
		client_session->next		= m_client_sessions;
		m_client_sessions			= client_session;
	}
	else {
		delete						client_session;
	}

	BOOST_INTERLOCKED_DECREMENT		( &s_active_connections );
	if ( m_connections_count-- >= m_max_connections_count ) {
		if ( m_connections_count < m_max_connections_count )
			start_client_acception	( );
	}
}

void login_server::start_client_acception		( )
{
	if ( m_connections_count >= m_max_connections_count ) {
		ASSERT						( m_connections_count == m_max_connections_count + 1 );
		return;
	}

	++m_connections_count;

	client_session* const new_client = new_client_session( );
	m_client_acceptor.async_accept	(
		new_client->socket(),
		make_custom_alloc_handler(
			m_socket_allocator,
			boost::bind(
				&login_server::process_client,
				this,
				new_client,
				boost::asio::placeholders::error
			)
		)
	);
}

void login_server::process_ping					(
		boost::system::error_code const& error,
		size_t const bytes_transferred
	)
{
	u32 session_id					= m_ping_buffer;
	start_ping_acception			( );

	if ( error )
		return;

	if ( bytes_transferred < sizeof(m_ping_buffer) )
		return;

	m_queries_server.set_last_activity_time		( session_id );
}

void login_server::start_ping_acception			( )
{
	m_ping_socket->async_receive_from(
		boost::asio::buffer( &m_ping_buffer, sizeof(m_ping_buffer) ),
		m_ping_endpoint,
		make_custom_alloc_handler(
			m_ping_allocator,
			boost::bind (
				&login_server::process_ping,
				this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred
			)
		)
	);
}

static long g_collected_count	= 0;

void login_server::collect_silently_disconnected_users	( )
{
	u64 const users_collected		= m_queries_server.disconnect_inactive( m_no_ping_interval );
	g_collected_count				+= static_cast< long >( users_collected );
	if ( users_collected )
		LOG_ERROR					( "collected %I64d silently disconnected online user%s\r\n", users_collected, users_collected == 1 ? "" : "s" );

	schedule_silently_disconnected_users_collection		( );
}

void login_server::schedule_silently_disconnected_users_collection	( )
{
	m_silently_disconnected_users_collection_timer.expires_from_now	( m_collection_routine_interval );
	m_silently_disconnected_users_collection_timer.async_wait		(
		make_custom_alloc_handler(
			m_collector_allocator,
			boost::bind( &login_server::collect_silently_disconnected_users, this )
		)
	);
}

long g_sign_up_count	= 0;
long g_sign_in_count	= 0;
long g_sign_out_count	= 0;

void login_server::test	( )
{
	LOG_ERROR						( "%d active connections: signed_up=%d signed_in=%d signed_out=%d collected=%d\r\n", s_active_connections, g_sign_up_count, g_sign_in_count, g_sign_out_count, g_collected_count );
	schedule_test					( );
}

void login_server::schedule_test				( )
{
	m_test.expires_from_now			( boost::posix_time::seconds(1) );
	m_test.async_wait				(
		make_custom_alloc_handler(
			m_collector_allocator,
			boost::bind( &login_server::test, this )
		)
	);
}

static long s_ping_processor_counter	= 0;
static long s_test_processor_counter	= 0;
static long s_collector_counter			= 0;

login_server::login_server						(
		boost::asio::io_service& io_service,
		short const port,
		boost::program_options::variables_map const& options
	) :
	m_queries_server				( m_connection, options ),
	m_client_acceptor				( io_service, tcp::endpoint( tcp::v4(), port ) ),
	m_ping_socket					( 0 ),
	m_silently_disconnected_users_collection_timer( io_service ),
	m_test							( io_service ),
#if XRAY_LOGIN_SERVER_USES_SSL
	m_ssl_context					( boost::asio::ssl::context::sslv23 ),
#endif // #if XRAY_LOGIN_SERVER_USES_SSL
	m_io_service					( io_service ),
	m_collection_routine_interval	( boost::posix_time::seconds( options["collect_interval"].as< u32 >() ) ),
	m_no_ping_interval				( boost::posix_time::seconds( options["collect_no_ping_period"].as< u32 >() ) ),
	m_client_sessions				( 0 ),
	m_connections_count				( 0 ),
	m_max_connections_count			( options["max_connections_count"].as< u32 >() ),
	m_reuse_client_sessions			( options["reuse_client_sessions"].as< bool >() )
{
#if XRAY_LOGIN_SERVER_USES_SSL
	if ( options["reuse_ssl_sessions"].as< bool >() )
		SSL_CTX_set_session_cache_mode	( m_ssl_context.native_handle(), SSL_SESS_CACHE_BOTH | SSL_SESS_CACHE_NO_AUTO_CLEAR );

    m_ssl_context.set_options		(
        boost::asio::ssl::context::default_workarounds
        | boost::asio::ssl::context::no_sslv2
        | boost::asio::ssl::context::single_dh_use
	);
    m_ssl_context.use_certificate_chain_file	( options["certification_chain_file"].as< std::string >() );
    m_ssl_context.use_private_key_file			( options["private_key_file"].as< std::string >(), boost::asio::ssl::context::pem );
    m_ssl_context.use_tmp_dh_file				( options["tmp_dh_file"].as< std::string >() );
#endif // #if XRAY_LOGIN_SERVER_USES_SSL

	LOG								( "connected to database successfully\r\n" );
	LOG								( "waiting for clients...\r\n" );

	if ( BOOST_INTERLOCKED_INCREMENT( &s_collector_counter ) == 1 ) {
		if ( options["collect_on_start_up"].as< bool >() )
			collect_silently_disconnected_users			( );
		else
			schedule_silently_disconnected_users_collection( );
	}

	if ( BOOST_INTERLOCKED_INCREMENT( &s_test_processor_counter ) == 1 ) {
		schedule_test				( );
	}
	
	start_client_acception			( );

	if ( BOOST_INTERLOCKED_INCREMENT( &s_ping_processor_counter ) == 1 ) {
		m_ping_socket				= new udp::socket( io_service, udp::endpoint( udp::v4(), port ) );
		start_ping_acception		( );
	}
}

login_server::~login_server						( )
{
	delete							m_ping_socket;

	for ( ; m_client_sessions; ) {
		client_session* const next_client_session	= m_client_sessions->next; 
		delete						m_client_sessions;
		m_client_sessions			= next_client_session;
	}
	
	mysql_close						( &m_connection );
}