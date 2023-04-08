////////////////////////////////////////////////////////////////////////////
//	Created		: 25.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef LOGIN_SERVER_H_INCLUDED
#define LOGIN_SERVER_H_INCLUDED

#include "query_processor.h"
#include "memory_allocator.h"

#if XRAY_LOGIN_SERVER_USES_SSL
#	include <boost/asio/ssl.hpp>
#endif // #if XRAY_LOGIN_SERVER_USES_SSL

namespace xray {

class client_session;

class login_server : private boost::noncopyable {
public:
			login_server				( boost::asio::io_service& io_service, short const port, boost::program_options::variables_map const& options );
			~login_server				( );
	
	void	delete_client_session		( client_session* client_session );

	inline	query_processor& get_queries_processor	( ) { return m_queries_server; }

private:
	void	process_client				( client_session* new_client, boost::system::error_code const& error );
	client_session* new_client_session	( );
	void	start_client_acception		( );

private:
	void	process_ping				( boost::system::error_code const& error, size_t const bytes_transferred );
	void	start_ping_acception		( );

private:
	void	collect_silently_disconnected_users				( );
	void	schedule_silently_disconnected_users_collection	( );
	void	test						( );
	void	schedule_test				( );

private:
	MYSQL								m_connection;
	query_processor						m_queries_server;
	tcp::acceptor						m_client_acceptor;
	udp::socket*						m_ping_socket;
	udp::endpoint						m_ping_endpoint;
	boost::asio::deadline_timer			m_silently_disconnected_users_collection_timer;
	boost::asio::deadline_timer			m_test;
#if XRAY_LOGIN_SERVER_USES_SSL
	boost::asio::ssl::context			m_ssl_context;
#endif // #if XRAY_LOGIN_SERVER_USES_SSL
	boost::posix_time::seconds const	m_collection_routine_interval;
	boost::posix_time::seconds const	m_no_ping_interval;
	handler_allocator					m_socket_allocator;
	handler_allocator					m_ping_allocator;
	handler_allocator					m_collector_allocator;
	boost::asio::io_service&			m_io_service;
	client_session*						m_client_sessions;
	u32									m_ping_buffer;
	u32									m_connections_count;
	u32 const							m_max_connections_count;
	bool const							m_reuse_client_sessions;
}; // class login_server

} // namespace xray

#endif // #ifndef LOGIN_SERVER_H_INCLUDED