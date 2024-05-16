////////////////////////////////////////////////////////////////////////////
//	Created		: [!output CURRENT_DATE]
//	Author		: [!output AUTHOR_NAME]
//	Copyright (C) GSC Game World - [!output CURRENT_YEAR]
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_[!output PROJECT_NAME_UPPER]_WORLD_H_INCLUDED
#define XRAY_[!output PROJECT_NAME_UPPER]_WORLD_H_INCLUDED

namespace xray {
namespace [!output PROJECT_NAME] {

struct XRAY_NOVTABLE world {
	virtual	void	tick		( ) = 0;

protected:
	XRAY_DECLARE_PURE_VIRTUAL_DESTRUCTOR( world );
}; // class world

} // namespace [!output PROJECT_NAME]
} // namespace xray

#endif // #ifndef XRAY_[!output PROJECT_NAME_UPPER]_WORLD_H_INCLUDED