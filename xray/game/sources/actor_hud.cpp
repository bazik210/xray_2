#include "pch.h"
#include "actor_hud.h"
#include "actor.h"
#include <xray/console_command.h>
#include <xray/console_command_processor.h>
#include <xray/render/facade/ui_renderer.h>

namespace stalker2 {

static bool g_draw_hud_value = true;
static console_commands::cc_bool g_draw_hud("hud_draw_ui", g_draw_hud_value, true, console_commands::command_type_user_specific);

actor_hud* actor_hud::get_instance()
{
    // TODO: to xray::uninitialized_reference
    static actor_hud s_actor_hud;
    return &s_actor_hud;
}

actor_hud::actor_hud() :
    m_wnd(nullptr),
    m_health_text(nullptr)
{
}

actor_hud::~actor_hud()
{
}

void actor_hud::init(stalker2::game& game)
{
    ui::world& ui_world = game.ui_world();
    float2 screen_size = ui_world.base_screen_size();

    m_wnd = ui_world.create_window();
    m_wnd->set_position(float2(0.f, 0.f));
    m_wnd->set_size(screen_size);
    m_wnd->set_visible(true);

    m_health_text = ui_world.create_text();
    m_health_text->w()->set_position(float2(0.5f, screen_size.y * 0.2f));
    m_health_text->w()->set_size(float2(100.f, 40.f));
    m_health_text->w()->set_visible(true);
    m_health_text->set_font(ui::fnt_arial);
    m_health_text->set_color(0xfff0f0f0);
    m_health_text->set_text_mode(ui::tm_default);

    m_wnd->add_child(m_health_text->w(), true);
}

void actor_hud::destroy(stalker2::game& game)
{
    ui::world& ui_world = game.ui_world();
    ui_world.destroy_window(m_health_text->w());
    ui_world.destroy_window(m_wnd);
}

void actor_hud::render(stalker2::game& game, stalker2::game_world& world)
{
    if (!g_draw_hud_value)
        return;

    if (!world.is_active() || world.is_loading_or_unloading())
        return;

    float player_health = world.m_local_actor->get_heath();

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "%.0f", player_health);
    m_health_text->set_text(buffer);
}

}