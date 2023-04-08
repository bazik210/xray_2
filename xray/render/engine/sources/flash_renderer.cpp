////////////////////////////////////////////////////////////////////////////
//	Created		: 07.03.2012
//	Author		: Plichko Alexander
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "flash_renderer.h"
#include <xray/render/core/device.h>
#include <xray/game/sources/flash_factory.h>

#pragma comment ( lib,"d3dx10.lib" )
#pragma comment	( lib,"d3d10.lib")

// GFx Includes
#include "GFx.h"
#include "GFx_Renderer_D3D1x.h"
#include "Render/D3D1x/D3D1x_HAL.h"

namespace xray {
namespace render {

flash_renderer::flash_renderer()
{
	LOG_INFO("flash_renderer::flash_renderer gfx heap is %x", Scaleform::Memory::GetGlobalHeap());
	m_HALRenderer = new Scaleform::Render::D3D1x::HAL();
	m_R2dRenderer = (new Scaleform::Render::Renderer2D(m_HALRenderer));
	R_ASSERT(m_R2dRenderer);
	
//	m_render_last_time = 0.f;
}

flash_renderer::~flash_renderer()
{
	//Scaleform::GFx::System::Destroy();
}

void flash_renderer::present( )
{
	if ( m_movies.empty() )
		return;
	vector<stalker2::flash_movie*>::const_iterator it = m_movies.begin();
	vector<stalker2::flash_movie*>::const_iterator it_e = m_movies.end();
	for ( ;it!=it_e;++it)
	{
		stalker2::flash_movie* movie = *it;
		Scaleform::GFx::MovieDisplayHandle handle = *movie->m_handle;

		m_R2dRenderer->BeginFrame();
		if (handle.NextCapture(m_R2dRenderer->GetContextNotify()))
		{
			m_R2dRenderer->Display(handle);
		}
		m_R2dRenderer->EndFrame();
	}
}

void flash_renderer::show_movie( stalker2::flash_movie* movie )
{
	m_movies.push_back(movie);
}

void flash_renderer::hide_movie( stalker2::flash_movie* movie )
{ 
	vector<stalker2::flash_movie*>::iterator it = std::find(m_movies.begin(), m_movies.end(), movie );
	R_ASSERT(it!=m_movies.end());
	m_movies.erase(it);
}
void flash_renderer::on_reset_device(  )
{
	LOG_INFO("gfx heap is %x", Scaleform::Memory::GetGlobalHeap());
	if ( !m_HALRenderer->IsInitialized())
		m_HALRenderer->InitHAL(
			Scaleform::Render::D3D1x::HALInitParams(
				device::ref().d3d_device(),
				device::ref().d3d_context(),
				Scaleform::Render::D3D1x::HALConfig_DynamicShaderCompile
			)
		);
	else
		m_HALRenderer->RestoreAfterReset();

}
void flash_renderer::on_create_device( ) 
{

}
void flash_renderer::on_lost_device( )
{

}






} // namespace render
} // namespace xray