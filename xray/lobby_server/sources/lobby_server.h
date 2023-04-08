////////////////////////////////////////////////////////////////////////////
//	Created		: 25.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef LOBBY_SERVER_H_INCLUDED
#define LOBBY_SERVER_H_INCLUDED

namespace xray {

class client_session;

class lobby_server : private boost::noncopyable {
public:
			lobby_server		( boost::asio::io_service& io_service, short const port );
			~lobby_server		( );

private:
	void	start_accept		( );
	void	handle_accept		( client_session* new_client, boost::system::error_code const& error );

private:
	boost::asio::ip::tcp::acceptor	acceptor_;
	MYSQL							connection_;
	boost::asio::io_service&		io_service_;
}; // class lobby_server

} // namespace xray

#endif // #ifndef LOBBY_SERVER_H_INCLUDED