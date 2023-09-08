////////////////////////////////////////////////////////////////////////////
//	Created		: 24.03.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

#include "main_menu.h"
#include "game.h"
#include "game_world.h"

#ifdef XRAY_STATIC_LIBRARIES
#ifdef XRAY_RENDERER_FLASH && GFX_SCALEFORM
	#include "flash_factory.h"
	#include "GFx.h"
#endif
#endif //#ifdef XRAY_STATIC_LIBRARIES

#include <xray/ui/world.h>
#include <xray/input/keyboard.h>
#include <xray/input/world.h>
#include <xray/render/facade/game_renderer.h>
#include <xray/render/facade/scene_renderer.h>



namespace stalker2 {


#ifndef XRAY_STATIC_LIBRARIES
//--------------------------------------------------------------------------------------
// FSCommand Handler
//--------------------------------------------------------------------------------------

class main_menu_fs_command_handler : public Scaleform::GFx::FSCommandHandler
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

class main_menu_external_handler : public Scaleform::GFx::ExternalInterface, private boost::noncopyable
{
private:
	game& m_game;
	game_world& m_game_world;
public:
	main_menu_external_handler( game& game, game_world& w ):m_game(game),m_game_world(w){}
	virtual void Callback(Scaleform::GFx::Movie* /*pmovieView*/,
		const char* methodName,
		const Scaleform::GFx::Value* args,
		unsigned /*argCount*/)
	{
		if (strcmp( methodName, "MenuButtonClicked" ) == 0){
			m_game.scene_close_query( );
			const char* command = args[0].GetString();
			if (strcmp( command, "FreeFly" ) == 0)
			{
				m_game_world.switch_to_free_fly_camera();
			}
			else if (strcmp( command, "Physics" ) == 0)
			{
//				m_game.get_game_world().test_physics1();
//				m_game.scene_close_query( );
			}
			else if (strcmp( command, "Hud" ) == 0)
			{
				m_game_world.switch_to_hud_camera();
			}
			else if (strcmp( command, "Quit" ) == 0)
			{
				m_game.exit			( "quit" );
			}		
		}

	}
};

void main_menu::create_main_menu_ui( )
{
	m_main_menu_ui = get_game().get_flash_factory().create_movie("../../resources/sources/flash_movies/test.swf");
	m_main_menu_ui->m_movie->SetBackgroundAlpha(0.0f);

	// Register our FSCommand handler
	Scaleform::Ptr<Scaleform::GFx::FSCommandHandler> pcommandHandler = *new main_menu_fs_command_handler;
	m_main_menu_ui->m_movie->SetFSCommandHandler(pcommandHandler);

	// Register our ExternalInterface handler
	Scaleform::Ptr<Scaleform::GFx::ExternalInterface> pEIHandler = *new main_menu_external_handler(get_game(), m_game_world);
	m_main_menu_ui->m_movie->SetExternalInterface(pEIHandler);


}

#endif //#ifdef XRAY_STATIC_LIBRARIES



struct main_menu_ui :public boost::noncopyable
{
main_menu_ui(xray::ui::world& ui_world)
:m_ui_world(ui_world)
{
	m_ui_dialog						= m_ui_world.create_dialog( );
	m_ui_dialog->w()->set_position	( float2(100, 100) );
	m_ui_dialog->w()->set_size		( float2(500,500) );

	ui::image* img					= m_ui_world.create_image( );
	img->init_texture				( "ui_rect" );
	img->set_color					( 0xff409040 );
	img->w()->set_size				( m_ui_dialog->w()->get_size() );
	img->w()->set_position			( float2(0, 0) );
	img->w()->set_visible			( true );
	m_ui_dialog->w()->add_child		( img->w(), true );

	ui::text* text					= m_ui_world.create_text( );
	text->set_font					( ui::fnt_arial );
	text->set_text					( "Main menu" );
	text->w()->set_size				( float2(200, 40) );
	text->w()->set_position			( float2(20, 120) );
	text->w()->set_visible			( true );
	m_ui_dialog->w()->add_child		( text->w(), true );

	text							= m_ui_world.create_text( );
	text->set_font					( ui::fnt_arial );
	text->set_text					( "ESC - switch to Game" );
	text->w()->set_size				( float2(200, 40) );
	text->w()->set_position			( float2(20, 140) );
	text->w()->set_visible			( true );
	m_ui_dialog->w()->add_child		( text->w(), true );

	text							= m_ui_world.create_text( );
	text->set_font					( ui::fnt_arial );
	text->set_text					( "Q - quit" );
	text->w()->set_size				( float2(200, 40) );
	text->w()->set_position			( float2(20, 160) );
	text->w()->set_visible			( true );
	m_ui_dialog->w()->add_child		( text->w(), true );

	text							= m_ui_world.create_text( );
	text->set_font					( ui::fnt_arial );
	text->set_text					( "H/J- attach/detach hud camera" );
	text->w()->set_size				( float2(200, 40) );
	text->w()->set_position			( float2(20, 180) );
	text->w()->set_visible			( true );
	m_ui_dialog->w()->add_child		( text->w(), true );
}
	~main_menu_ui()
	{
		m_ui_world.destroy_window(m_ui_dialog->w());
	}
	xray::ui::world&		m_ui_world;
	ui::dialog*				m_ui_dialog;
};


main_menu::main_menu( game& g, game_world& w ) 
:super			( g ),
m_game_world	( w )
{
#ifndef MASTER_GOLD
	m_dbg_name							= "Main Menu";
#endif //#ifndef MASTER_GOLD

	query_resources					( );

#ifndef XRAY_STATIC_LIBRARIES
	create_main_menu_ui		();
#endif //#ifdef XRAY_STATIC_LIBRARIES

}


main_menu::~main_menu( )
{
#ifndef XRAY_STATIC_LIBRARIES
	DELETE( m_main_menu_ui );
#endif //#ifdef XRAY_STATIC_LIBRARIES
}

void main_menu::on_deactivate( )
{
	super::on_deactivate				( );
	get_game().input_world().remove_handler	( *this );
#ifndef XRAY_STATIC_LIBRARIES
	#ifdef XRAY_RENDERER_FLASH
		renderer().hide_movie( get_game().render_output_window(), m_main_menu_ui );	
	#endif
#else	
	DELETE	(m_ui);
#endif //#ifdef XRAY_STATIC_LIBRARIES
}

#ifdef XRAY_STATIC_LIBRARIES
input::handler*	main_menu::dialog_input_handler	()
{
	return m_ui->m_ui_dialog->input_handler();
}
#endif //#ifdef XRAY_STATIC_LIBRARIES


void main_menu::on_activate( )
{
	super::on_activate					( );
	get_game().input_world().add_handler	( *this );

#ifndef XRAY_STATIC_LIBRARIES
	#ifdef XRAY_RENDERER_FLASH
		m_window_size = renderer().scene().window_client_size(get_game().render_output_window());
		m_main_menu_ui->m_movie->SetViewport(m_window_size.width, m_window_size.height, 0, 0, m_window_size.width, m_window_size.height, 0);
		renderer().show_movie( get_game().render_output_window(), m_main_menu_ui );
		m_main_menu_ui_last_time = m_timer.get_elapsed_msec();
	#endif
#else
	m_ui	= NEW(main_menu_ui)(m_game.ui_world());
#endif //#ifdef XRAY_STATIC_LIBRARIES
}


void main_menu::tick( )
{
#ifndef XRAY_STATIC_LIBRARIES
	if (m_main_menu_ui){

		u32 current_time = m_timer.get_elapsed_msec();
		
		float deltaTime = ((float)(current_time - m_main_menu_ui_last_time)) / 1000.0f;
		//LOG_INFO("TIME_DELTA: [%f] --- [%d]:[%d]", deltaTime, current_time, m_main_menu_ui_last_time );
		m_main_menu_ui_last_time = current_time;
		
		m_main_menu_ui->m_movie->Advance( deltaTime, 0 );		
	}
#else
	m_ui->m_ui_dialog->w()->tick( );
	m_ui->m_ui_dialog->w()->draw( m_game.ui_world().get_renderer(), get_render_scene_view() );
#endif //#ifdef XRAY_STATIC_LIBRARIES
}

void main_menu::query_resources( )
{
	xray::render::scene_configuration				render_configuration;
	render_configuration.m_create_terrain			= false;
	render_configuration.m_create_particle_world	= false;
	
	resources::user_data_variant temp_data;
	temp_data.set				( render_configuration );
	
	xray::resources::user_data_variant const* data[] = { &temp_data, 0 };
	
	xray::const_buffer			temp_buffer( "", 1 );
	xray::resources::creation_request requests[] = 
	{
		xray::resources::creation_request( "game_scene", temp_buffer, resources::scene_class ),
		xray::resources::creation_request( "game_scene_view", temp_buffer, resources::scene_view_class ),
	};
 	xray::resources::query_create_resources(
 		requests,
		boost::bind( &main_menu::on_resources_ready, this, _1 ),
 		stalker2::g_allocator,
 		data
 	);
}

void main_menu::on_resources_ready( xray::resources::queries_result& data )
{
	m_scene						= static_cast_resource_ptr< xray::render::scene_ptr >( data[0].get_unmanaged_resource() );
	m_scene_view				= static_cast_resource_ptr< xray::render::scene_view_ptr >( data[1].get_unmanaged_resource() );
}
} // namespace stalker2
