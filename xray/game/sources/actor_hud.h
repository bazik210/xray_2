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
	namespace game { class renderer; }

} // namespace render
}

namespace stalker2 {

class game;
class game_world;

class actor_hud
{
public:
	static actor_hud* get_instance();

public:
	actor_hud();
	~actor_hud();

	void init(stalker2::game& game);
	void destroy(stalker2::game& game);

	void render(stalker2::game& game, stalker2::game_world& world);
private:
	ui::window* m_wnd;
	ui::text* m_health_text;
};

}

#endif // !ACTOR_HUD_H
