#include "pch.h"
#include "actor_hud.h"
#include "actor.h"
#include <xray/console_command.h>
#include <xray/console_command_processor.h>
#include <xray/render/facade/ui_renderer.h>

namespace stalker2 {

static bool g_draw_hud_value = true;
static console_commands::cc_bool g_draw_hud("hud_draw_ui", g_draw_hud_value, true, console_commands::command_type_user_specific);

actor_hud::actor_hud(xray::ui::world* ui_world, stalker2::game_world* m_world, stalker2::game* game) :
    m_ui_world(ui_world),
    m_game(game),
    m_world(m_world),
    m_wnd(nullptr),
    m_health_text(nullptr)
{
    m_ui_world = &m_game->ui_world();
    float2 screen_size = m_ui_world->base_screen_size();

    m_wnd = m_ui_world->create_window();
    m_wnd->set_position(float2(0.f,0.f));
    m_wnd->set_size(screen_size);
    m_wnd->set_visible(true);

    ui::image* m_img			= m_ui_world->create_image();
	m_img->init_texture				( "ui_health" );
	m_img->set_color				( 0xfff0f0f0 );
	m_img->w()->set_size			( float2(128, 64) );
    if (!m_game->engine().command_line_editor())
    {
        m_img->w()->set_position(float2(100, screen_size.y - 100));
    }
    else {
        m_img->w()->set_position(float2(100, screen_size.y * 3 - 100));
    }
	m_img->w()->set_visible			( true );
	m_wnd->add_child		        ( m_img->w(), true );

    m_health_text = m_ui_world->create_text();
    if (!m_game->engine().command_line_editor())
    {
        m_health_text->w()->set_position(float2(130, screen_size.y - 78));
    }
    else {
        m_health_text->w()->set_position(float2(130, screen_size.y * 3 - 78));
    }
    //m_health_text->w()->set_size(float2(100.f, 40.f));
    m_health_text->w()->set_visible(true);
    m_health_text->set_font(ui::fnt_arial);
    m_health_text->set_color(0xfff0f0f0);
    m_health_text->set_text_mode(ui::tm_default);
    m_wnd->add_child(m_health_text->w(), true);
}

actor_hud::~actor_hud()
{
    m_ui_world->destroy_window(m_wnd);

    m_wnd = 0;
}

void actor_hud::render(render::ui::renderer& w, xray::render::scene_view_ptr const& scene_view)
{
    if (!g_draw_hud_value)
        return;

    if (!m_wnd || !m_world || !m_world->is_active())
        return;

    float player_health = m_world->m_local_actor->get_heath();

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%.0f", player_health);
    m_health_text->set_text(buffer);

    m_wnd->draw(w, scene_view);
}

}