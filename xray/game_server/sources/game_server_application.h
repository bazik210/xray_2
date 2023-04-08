////////////////////////////////////////////////////////////////////////////
//	Created		: 27.03.2012
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_SERVER_APPLICATION_H_INCLUDED
#define GAME_SERVER_APPLICATION_H_INCLUDED

namespace xray {

namespace network {
	struct world;
	class server;
} // namespace network

namespace game_server {

class application {
public:
			void	initialize		( );
			void	execute			( );
			void	finalize		( );
	inline	u32		get_exit_code	( ) const	{ return m_exit_code; }

private:
	xray::network::world*	m_network;
	xray::network::server*	m_server;
	u32						m_exit_code;
}; // class application

} // namespace game_server
} // namespace xray

#endif // #ifndef GAME_SERVER_APPLICATION_H_INCLUDED