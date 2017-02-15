#include "reducer_world.hpp"
#include "entity_state.hpp"
#include "game_state.hpp"

namespace te {

void inputEntity(entitystate_t& state, const Uint8 *keyboard, const gamestate_t& game) {
	static constexpr float SPEED = 3 * 32.0f;
	state.velocities[game.world.playerEntity].x = (keyboard[SDL_SCANCODE_D] - keyboard[SDL_SCANCODE_A]) * SPEED;
}

}
