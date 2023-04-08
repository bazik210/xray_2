////////////////////////////////////////////////////////////////////////////
//	Created		: 20.09.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "render_output_window.h"
#include <xray/render/core/resource_manager.h>
#include <xray/render/core/device.h>
#include <xray/render/core/render_target.h>
#include <xray/render/core/backend.h>
#include <xray/render/facade/common_types.h>
#include "flash_renderer.h"

namespace xray {
namespace render {

render_output_window::render_output_window( xray::render::output_window_configuration const& window_configuration )
:	m_targets		( get_window_client_size((HWND)window_configuration.m_hwnd) ),
	m_window		( (HWND)window_configuration.m_hwnd ),
	m_output		( resource_manager::ref().create_render_output( (HWND)window_configuration.m_hwnd, true) ),
	m_current_size	( get_window_client_size((HWND)window_configuration.m_hwnd) )
{
	if(window_configuration.m_create_flash_renderer)
	{
		m_flash_renderer								= NEW(flash_renderer)();
		backend::ref().set_render_targets	( &*m_targets.m_rt_generic_0, 0, 0, 0);
		backend::ref().flush();
		m_flash_renderer->on_reset_device();
	}else
		m_flash_renderer								= NULL;

}

void render_output_window::resize		( )
{
	math::uint2 const& new_size = get_window_client_size(m_window);
	if ( (new_size.x == 0) || (new_size.y == 0) )
		return;

	if ( (new_size.x == m_current_size.x) && (new_size.y == m_current_size.y) )
		return;

	m_current_size	= new_size;
	m_targets.resize( new_size );
	m_output->resize( );
	if(m_flash_renderer)
		m_flash_renderer->on_reset_device();
}
//DELETE			(m_flash_renderer );

math::uint2	render_output_window::get_window_client_size( HWND const window )
{
	RECT			rect;
	if ( GetClientRect( window, &rect) )
		return		math::uint2(rect.right-rect.left, rect.bottom-rect.top);

	return			math::uint2( 0, 0 );
}

} // namespace render
} // namespace xray