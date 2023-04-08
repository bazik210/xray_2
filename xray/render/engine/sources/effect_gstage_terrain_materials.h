////////////////////////////////////////////////////////////////////////////
//	Created		: 19.03.2012
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#ifndef EFFECT_GSTAGE_TERRAIN_MATERIALS_H_INCLUDED
#define EFFECT_GSTAGE_TERRAIN_MATERIALS_H_INCLUDED

#include "effect_material_base.h"

namespace xray {
namespace render {

class effect_gstage_terrain_materials: public effect_material_base
{
public:
	virtual void compile( effect_compiler& compiler, const custom_config_value& custom_config);
}; // class effect_gstage_terrain_materials

} // namespace render 
} // namespace xray 


#endif // #ifndef EFFECT_GSTAGE_TERRAIN_MATERIALS_H_INCLUDED
