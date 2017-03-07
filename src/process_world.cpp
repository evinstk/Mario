#include "game_action.hpp"
#include "world_state.hpp"
#include "game_state.hpp"

namespace te {

void worldstate_t::dispatch() {
	static constexpr float JUMP_SPEED = -10 * 32;

	for (playercmd_t cmd : pGame->command.commandQueue) {
		if (cmd == playercmd_t::JUMP) {
			entity.velocities[playerEntity].y = JUMP_SPEED;
		}
	}
}

} // namespace te
