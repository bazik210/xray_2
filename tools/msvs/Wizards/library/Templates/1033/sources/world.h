////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#ifndef [!output PROJECT_NAME_UPPER]_WORLD_H_INCLUDED
#define [!output PROJECT_NAME_UPPER]_WORLD_H_INCLUDED

#include <xray/[!output PROJECT_NAME]/world.h>

namespace xray {
namespace [!output PROJECT_NAME] {

struct engine;

class [!output PROJECT_NAME]_world :
	public [!output PROJECT_NAME]::world,
	private boost::noncopyable
{
public:
					[!output PROJECT_NAME]_world	( [!output PROJECT_NAME]::engine& engine );
	virtual			~[!output PROJECT_NAME]_world	( );
	virtual	void	tick			( );

private:
	engine&	m_engine;
}; // class [!output PROJECT_NAME]_world

} // namespace [!output PROJECT_NAME]
} // namespace xray

#endif // #ifndef [!output PROJECT_NAME_UPPER]_WORLD_H_INCLUDED