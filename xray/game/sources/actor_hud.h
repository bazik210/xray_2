#ifndef ACTOR_HUD_H
#define ACTOR_HUD_H

#include <xray/engine/engine_user.h>
#include <xray/input/engine.h>
#include <xray/ui/ui.h>
#include <xray/ui/engine.h>
#include <xray/ui/world.h>
#include <xray/render/engine/base_classes.h>
#include "game.h"
#include "game_world.h"

namespace xray {

namespace ui { struct world; }
namespace rtp { struct world; }
namespace physics { struct world; }
namespace input { struct world; }
namespace collision { struct space_partitioning_tree; }
namespace animation { struct world; }
namespace sound { class sound_debug_stats; }

namespace render {
	namespace ui { class renderer; }

} // namespace render
}

namespace stalker2 {

class game;
class game_world;

class actor_hud
{
public:
	actor_hud(xray::ui::world* ui_world, stalker2::game_world* m_world, stalker2::game* game);
	~actor_hud();

	void render(xray::render::ui::renderer& w, xray::render::scene_view_ptr const& scene);

private:
	ui::world* m_ui_world;
	stalker2::game* m_game;
	stalker2::game_world* m_world;
	ui::window* m_wnd;
	ui::text* m_health_text;
};

}

#endif // !ACTOR_HUD_H
