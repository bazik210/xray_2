////////////////////////////////////////////////////////////////////////////
//	Created		: 14.12.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_RENDER_ENGINE_SCENE_XBOX360_H_INCLUDED
#define XRAY_RENDER_ENGINE_SCENE_XBOX360_H_INCLUDED

//#include <xray/particle/engine.h>
//#include <xray/render/engine/vertex_colored.h>
#include <xray/render/engine/base_classes.h>
//#include "render_model.h"
//#include "decal_instance.h"


namespace xray {
namespace render {

struct editor_renderer_configuration;

class scene : public base_scene {
public:
								scene					( editor_renderer_configuration const& renderer_configuration );
}; // class scene


} // namespace render
} // namespace xray

#endif // #ifndef XRAY_RENDER_ENGINE_SCENE_XBOX360_H_INCLUDED