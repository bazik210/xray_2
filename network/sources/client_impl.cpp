////////////////////////////////////////////////////////////////////////////
//	Created		: 21.02.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "client_impl.h"
#include "client_error_codes.h"
#include <xray/network/packet.h>

using xray::network::client_impl;
using boost::asio::ip::tcp;

client_impl::client_impl			( boost::asio::io_service& io_service, xray::network::network_world& world ) :
	m_socket						( io_service ),
	m_packet_socket					( m_socket, world ),
	m_io_service					( io_service ),
	m_connection_state				( host_name_is_unresolved )
{
	m_packet_socket.set_on_error	( boost::bind(&client_impl::on_error, this, _1, _2) );
}

client_impl::~client_impl			( )
{
	if ( !is_connected() )
		return;

	disconnect						( );
}

void client_impl::start_reading		( )
{
	m_packet_socket.start_receiving	( );
}

void client_impl::on_connected		(
		boost::system::error_code const& error_code,
		tcp::resolver::iterator iterator
	)
{
	ASSERT							( m_connection_state == connection_is_being_established );

	if ( error_code ) {
		m_connection_state			= host_name_is_unresolved;
		if ( m_on_error )
			m_on_error				( server_cannot_be_connected, error_code );
		return;
	}

	LOG_INFO						( "connection_has_been_established!" );
	m_connection_state				= connection_has_been_established;

	if ( m_on_connected )
		m_on_connected				( );

	start_reading					( );
}

void client_impl::connect			( tcp::resolver::iterator const& iterator )
{
	m_connection_state				= connection_is_being_established;
	boost::asio::async_connect		(
		m_socket,
		iterator,
		boost::bind(
			&client_impl::on_connected,
			this,
			_1,
			_2
		)
	);
}

void client_impl::on_resolved		(
		tcp::resolver* resolver,
		boost::system::error_code const& error_code,
		tcp::resolver::iterator iterator
	)
{
	ASSERT							( m_connection_state == host_name_is_being_resolved );

	if ( error_code ) {
		LOG_INFO					( "NOT host_name_has_been_resolved!" );
		LOG_INFO					( "error during host_name_is_being_resolved: %s", error_code.message().c_str() );
		++iterator;
		if ( iterator != tcp::resolver::iterator() ) {
			resolver->async_resolve	(
				*iterator,
				boost::bind(
					&client_impl::on_resolved,
					this,
					resolver,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred
				)
			);
			return;
		}

		XRAY_DELETE_IMPL			( g_allocator, resolver );
		m_connection_state			= host_name_is_unresolved;
		LOG_INFO					( "can't resolve endpoints: %s", error_code.message().c_str() );
		LOG_INFO					( "please, try again later" );
		if ( m_on_error )
			m_on_error				( host_cannot_be_resolved, error_code );
		return;
	}

	XRAY_DELETE_IMPL				( g_allocator, resolver );
	LOG_INFO						( "host_name_has_been_resolved!" );
	m_connection_state				= host_name_has_been_resolved;
	m_host							= iterator;
	connect							( iterator );
}

void client_impl::resolve			( pcstr const host, u32 const host_port )
{
	LOG_INFO						( "host_name_is_being_resolved..." );

	ASSERT							( m_connection_state == host_name_is_unresolved );
	m_connection_state				= host_name_is_being_resolved;

	tcp::resolver* const resolver	= NEW( tcp::resolver) ( m_io_service );

	char port[6];
	_itoa_s							( host_port, port, 10 );

	tcp::resolver::query query( tcp::v4(), host, port );
	resolver->async_resolve			(
		query,
		boost::bind(
			&client_impl::on_resolved,
			this,
			resolver,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

void client_impl::connect			( pcstr const host, u16 const port )
{
	resolve							( host, port );
}

void client_impl::disconnect		( )
{
	m_packet_socket.stop_receiving	( );
	close_connection				( );
}

void client_impl::close_connection	( )
{
	m_socket.shutdown				( boost::asio::socket_base::shutdown_both );
	m_socket.close					( );
}

void client_impl::send				( xray::network::packet const& packet )
{
	m_packet_socket.send			( packet );
}

void client_impl::on_error			( xray::network::client_error_codes_enum const client_error_code, boost::system::error_code const error_code )
{
	m_connection_state				= host_name_is_unresolved;
	if ( m_on_error )
		m_on_error					( client_error_code, error_code );
}