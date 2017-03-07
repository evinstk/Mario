#include "game_action.hpp"
#include "world_state.hpp"
#include "game_state.hpp"

namespace te {

void dispatchWorld(worldstate_t& state, const gamestate_t& game) {
	static constexpr float JUMP_SPEED = -10 * 32;

	for (playercmd_t cmd : game.command.commandQueue) {
		if (cmd == playercmd_t::JUMP) {
			state.entity.velocities[state.playerEntity].y = JUMP_SPEED;
		}
	}
}

} // namespace te
