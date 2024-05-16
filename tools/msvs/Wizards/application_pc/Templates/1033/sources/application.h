////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#ifndef [!output PROJECT_NAME_UPPER]_APPLICATION_H_INCLUDED
#define [!output PROJECT_NAME_UPPER]_APPLICATION_H_INCLUDED

#include <xray/stalker2/game_module_proxy.h>

namespace [!output PROJECT_NAME] {

class application {
public:
			void		initialize		( );
			void		execute			( );
			void		finalize		( );
	inline	u32			get_exit_code	( ) const	{ return m_exit_code; }
[!output INITIALIZE_H]
private:
	stalker2::game_module_proxy	m_game_proxy;
	u32					m_exit_code;
}; // class application

} // namespace [!output PROJECT_NAME]

#endif // #ifndef [!output PROJECT_NAME_UPPER]_APPLICATION_H_INCLUDED