#include "game_action.hpp"
#include "world_state.hpp"
#include "game_state.hpp"
#include <SDL.h>

namespace te {

void processWorld(worldstate_t& state, const SDL_Event& evt, const gamestate_t& game) {
	static constexpr float JUMP_SPEED = -10 * 32;

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_SPACE &&
			evt.key.repeat == 0 &&
			game.world.entity.groundOffsets.find(state.playerEntity) != game.world.entity.groundOffsets.end()) {

			state.entity.velocities[state.playerEntity].y = JUMP_SPEED;
		}
	}
}

} // namespace te
