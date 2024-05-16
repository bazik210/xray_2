////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_[!output PROJECT_NAME_UPPER]_API_H_INCLUDED
#define XRAY_[!output PROJECT_NAME_UPPER]_API_H_INCLUDED

#ifndef XRAY_[!output PROJECT_NAME_UPPER]_API
#	ifdef XRAY_STATIC_LIBRARIES
#		define XRAY_[!output PROJECT_NAME_UPPER]_API
#	else // #ifdef XRAY_STATIC_LIBRARIES
#		ifdef XRAY_[!output PROJECT_NAME_UPPER]_BUILDING
#			define XRAY_[!output PROJECT_NAME_UPPER]_API		XRAY_DLL_EXPORT
#		else // #ifdef XRAY_[!output PROJECT_NAME_UPPER]_BUILDING
#			ifndef XRAY_ENGINE_BUILDING
#				define XRAY_[!output PROJECT_NAME_UPPER]_API	XRAY_DLL_IMPORT
#			else // #ifndef XRAY_ENGINE_BUILDING
#				define XRAY_[!output PROJECT_NAME_UPPER]_API	XRAY_DLL_EXPORT
#			endif // #ifndef XRAY_ENGINE_BUILDING
#		endif // #ifdef XRAY_[!output PROJECT_NAME_UPPER]_BUILDING
#	endif // #ifdef XRAY_STATIC_LIBRARIES
#endif // #ifndef XRAY_[!output PROJECT_NAME_UPPER]_API

namespace xray {
namespace [!output PROJECT_NAME] {

typedef memory::doug_lea_allocator_type		allocator_type;

struct world;
struct engine;

XRAY_[!output PROJECT_NAME_UPPER]_API	world*	create_world	( engine& engine );
XRAY_[!output PROJECT_NAME_UPPER]_API	void	destroy_world	( world*& world );
XRAY_[!output PROJECT_NAME_UPPER]_API	void	memory_allocator( allocator_type& allocator );

} // namespace [!output PROJECT_NAME]
} // namespace xray

#endif // #ifndef XRAY_[!output PROJECT_NAME_UPPER]_API_H_INCLUDED