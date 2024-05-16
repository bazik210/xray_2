////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#ifndef [!output PROJECT_NAME_UPPER]_APPLICATION_H_INCLUDED
#define [!output PROJECT_NAME_UPPER]_APPLICATION_H_INCLUDED

namespace xray {
namespace [!output PROJECT_NAME] {

class application {
public:
			void	initialize		( );
			void	execute			( );
			void	finalize		( );
	inline	u32		get_exit_code	( ) const	{ return m_exit_code; }

private:
	u32		m_exit_code;
}; // class application

} // namespace [!output PROJECT_NAME]
} // namespace xray

#endif // #ifndef [!output PROJECT_NAME_UPPER]_APPLICATION_H_INCLUDED