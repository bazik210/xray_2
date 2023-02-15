////////////////////////////////////////////////////////////////////////////
//	Created		: 09.11.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef XRAY_ENGINE_CONSOLE_H_INCLUDED
#define XRAY_ENGINE_CONSOLE_H_INCLUDED

#include <xray/input/handler.h>
#include <xray/render/engine/base_classes.h>

namespace xray {
namespace engine {

class console : public xray::input::handler
{
public:
	virtual					~console			( )			{};
	virtual bool			get_active			( ) const	= 0;
	virtual void			on_activate			( )			= 0;
	virtual void			on_deactivate		( )			= 0;
	virtual void			tick				( xray::render::scene_view_ptr const& scene_view )			= 0;
}; // class console

} // namespace engine
} // namespace xray

#endif // #ifndef XRAY_ENGINE_CONSOLE_H_INCLUDED