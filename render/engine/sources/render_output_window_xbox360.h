////////////////////////////////////////////////////////////////////////////
//	Created		: 14.12.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_RENDER_ENGINE_RENDER_OUTOUT_WINDOW_XBOX360_H_INCLUDED
#define XRAY_RENDER_ENGINE_RENDER_OUTOUT_WINDOW_XBOX360_H_INCLUDED


#include "renderer_context_targets.h"
#include <xray/render/engine/base_classes.h>

namespace xray {
namespace render {

class render_output_window: public base_output_window {
public:
	render_output_window( HWND window ) {}
}; // class render_output_window

} // namespace render
} // namespace xray

#endif // #ifndef XRAY_RENDER_ENGINE_RENDER_OUTOUT_WINDOW_H_INCLUDED