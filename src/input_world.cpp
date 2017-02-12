#include "reducer_world.hpp"
#include "world_state.hpp"
#include <SDL.h>

namespace te {

void inputWorld(worldstate_t& state, const SDL_Event& evt) {
	static constexpr float JUMP_SPEED = -6.5f;

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_SPACE) {
			state.entity.velocities[state.playerEntity].y = JUMP_SPEED;
		}
	}
}

} // namespace te
