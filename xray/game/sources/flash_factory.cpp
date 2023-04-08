////////////////////////////////////////////////////////////////////////////
//	Created		: 07.03.2012
//	Author		: Plichko Alexander
//	Copyright (C) GSC Game World - 2012
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game.h"
#include "flash_factory.h"

#define GFX_AS3_SUPPORT

// GFx Includes
#include "GFx_Kernel.h"
#include "GFx.h"
#include "GFx/AS3/AS3_Global.h"
#include "Render/ImageFiles/PNG_ImageFile.h"
#include "Render/ImageFiles/DDS_ImageFile.h"

namespace stalker2 
{

//--------------------------------------------------------------------------------------
// FSCommand Handler
//--------------------------------------------------------------------------------------

class flash_fs_command_handler : public Scaleform::GFx::FSCommandHandler
{
public:
	virtual void Callback(Scaleform::GFx::Movie* /*pmovie*/,
		const char* /*pcommand*/, const char* /*parg*/)
	{

	}
};

//--------------------------------------------------------------------------------------
// ExternalInterface Handler
//--------------------------------------------------------------------------------------

class flash_external_handler : public Scaleform::GFx::ExternalInterface
{
public:
	virtual void Callback(Scaleform::GFx::Movie* /*pmovieView*/,
		const char* /*methodName*/,
		const Scaleform::GFx::Value* /*args*/,
		unsigned /*argCount*/)
	{

	}
};


flash_factory::flash_factory( game& game )
:m_game(game)
{
	LOG_INFO("gfx heap is %x", Scaleform::Memory::GetGlobalHeap());
	m_gfx_loader = NEW( Scaleform::GFx::Loader )();


	// Initialize logging -- GFx will print errors to the log
	// stream. GFx log messages are very helpful when
	// debugging. To generate a simpele error, comment out
	// gfxLoader.SetFileOpener(pfileOpener).
	//m_gfx_loader.SetLog(Ptr<SF::Log>(*new GFxPlayerLog()));

	// Give the loader the default file opener
	Scaleform::Ptr<Scaleform::GFx::FileOpener> pfileOpener = *new Scaleform::GFx::FileOpener;
	m_gfx_loader->SetFileOpener(pfileOpener);

	// Register our FSCommand handler
	Scaleform::Ptr<Scaleform::GFx::FSCommandHandler> pcommandHandler = *new flash_fs_command_handler;
	m_gfx_loader->SetFSCommandHandler(pcommandHandler);

	// Register our ExternalInterface handler
	Scaleform::Ptr<Scaleform::GFx::ExternalInterface> pEIHandler = *new flash_external_handler;
	m_gfx_loader->SetExternalInterface(pEIHandler);

	// Enabling AS2 support. These two lines could be commented out if 
	// AS2 support is not required.
	Scaleform::Ptr<Scaleform::GFx::ASSupport> pAS2Support = *new Scaleform::GFx::AS2Support();
	m_gfx_loader->SetAS2Support(pAS2Support);

	// Enabling AS3 support. These two lines could be commented out if 
	// AS3 support is not required; comment out #include "GFx/AS3/AS3_Global.h"
	// above too in this case.
	Scaleform::Ptr<Scaleform::GFx::ASSupport> pASSupport = *new Scaleform::GFx::AS3Support();
	m_gfx_loader->SetAS3Support(pASSupport);

	// Enabling image support, JPEG, PNG, TGA and DDS formats.
	Scaleform::Ptr<Scaleform::GFx::ImageFileHandlerRegistry> pImgReg = *new Scaleform::GFx::ImageFileHandlerRegistry();
	pImgReg->AddHandler(&Scaleform::Render::JPEG::FileReader::Instance);
	pImgReg->AddHandler(&Scaleform::Render::PNG::FileReader::Instance);
	pImgReg->AddHandler(&Scaleform::Render::TGA::FileReader::Instance);
	pImgReg->AddHandler(&Scaleform::Render::DDS::FileReader::Instance);
	m_gfx_loader->SetImageFileHandlerRegistry(pImgReg);

	// Use EdgeAA to improve the appearance of the interface without the computational
	// expense of full AA through the video card.
	//pRenderConfig->SetRenderFlags(RenderConfig::RF_EdgeAA);
}

flash_factory::~flash_factory( )
{
	//Scaleform::GFx::System::Destroy();
//	DELETE(m_gfx_loader);
};

flash_movie* flash_factory::create_movie( char* file_name )
{
	LOG_INFO("gfx heap is %x", Scaleform::Memory::GetGlobalHeap());

	flash_movie* ret_movie = NEW(flash_movie)();

	// Load the movie and crate instance.
	ret_movie->m_movie_def = (m_gfx_loader->CreateMovie(file_name, Scaleform::GFx::Loader::LoadWaitFrame1, 0));
	R_ASSERT(ret_movie->m_movie_def);

	// We pass 'true' argument to make sure that first frame is initialized.
	// We can use 'false' if we need to pass some information to the movie
	// before its first frame 1 ActionScript logic is executed.
	ret_movie->m_movie = ret_movie->m_movie_def->CreateInstance( true, 0 );
	R_ASSERT(ret_movie->m_movie);

	//For GFx 3.3 and above, the controller and mouse count must be set
	//otherwise GFx will assume that neither are present
	ret_movie->m_movie->SetMouseCursorCount(1);
	ret_movie->m_movie->SetControllerCount(1);

	// get the display handle
	ret_movie->m_handle = &ret_movie->m_movie->GetDisplayHandle();

	// Advance the movie to the first frame
	ret_movie->m_movie->Advance(0.0f, 0);

	// Set the background stage color to alpha blend with the underlying 3D environment
	//ret_movie->m_movie->SetBackgroundAlpha(0.0f);

	// Set the scale mode to preserve the aspect ratio of the Flash content
	ret_movie->m_movie->SetViewScaleMode(Scaleform::GFx::Movie::SM_ShowAll);
	ret_movie->m_movie->SetViewAlignment(Scaleform::GFx::Movie::Align_CenterRight);

	return ret_movie;
}

void flash_factory::destroy_movie( char* /*file_name*/ )
{

}

void flash_factory::tick()
{
	

}



}; //namespace stalker2 