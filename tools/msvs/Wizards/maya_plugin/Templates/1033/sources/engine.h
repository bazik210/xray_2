////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#ifndef [!output PROJECT_NAME_UPPER]_ENGINE_H_INCLUDED
#define [!output PROJECT_NAME_UPPER]_ENGINE_H_INCLUDED

#include <xray/core/engine.h>

namespace xray {
namespace [!output PROJECT_NAME] {

class [!output PROJECT_NAME]_engine : public xray::core::engine {
public:
					[!output PROJECT_NAME]_engine				( );

	virtual void	exit					( int error_code )	{ m_exit_code = error_code; xray::debug::terminate(""); }
	virtual int		get_exit_code			( ) const			{ return m_exit_code; }
	virtual void	set_exit_code			( int error_code )	{ m_exit_code = error_code; }
	virtual	pcstr	get_resource_path		( ) const;
	virtual	pcstr	get_underscore_G_path	( ) const;

public:
	void			assign_resource_path	( );

private:
	string_path		m_resource_path;
	string_path		m_underscore_G_path;
	int				m_exit_code;
} ; // class [!output PROJECT_NAME]_engine

} // namespace [!output PROJECT_NAME]
} // namespace xray

#endif // #ifndef [!output PROJECT_NAME_UPPER]_ENGINE_H_INCLUDED