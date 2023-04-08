////////////////////////////////////////////////////////////////////////////
//	Created		: 25.01.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef CLIENT_SESSION_H_INCLUDED
#define CLIENT_SESSION_H_INCLUDED

namespace boost {
	namespace asio {
		class io_service;
	} // namespace asio

	namespace system {
		class error_code;
	} // namespace system
} // namespace boost

namespace xray {

class client_session : private boost::noncopyable {
public:
						client_session				( boost::asio::io_service& io_service, MYSQL& connection );
			void		start						( );
	inline	boost::asio::ip::tcp::socket& socket	( ) { return m_socket; }

private:
	typedef boost::function<
		void ( client_session*, boost::system::error_code const& error, size_t const bytes_transferred )
	> read_handler_type;

private:
			void 		read_async					( read_handler_type const& read_handler );
			void 		handle_write				( read_handler_type const& functor, boost::system::error_code const& error );
			void 		send_message				( lobby_client_message_types_enum message_type, read_handler_type const& functor );
			void 		send_lobby_info				( );
			void 		add_new_user				( );
			void 		process_sign_up				( );
			void 		process_sign_in				( );
			void 		process_client_initiation	( boost::system::error_code const& error, size_t const bytes_transferred );

private:
	enum {
		max_length		= 1024,
	};

private:
	boost::asio::ip::tcp::socket	m_socket;
	MYSQL&							connection_;
	char							data_[ max_length ];
}; // class client_session

} // namespace xray

#endif // #ifndef CLIENT_SESSION_H_INCLUDED