#include "reducer_world.hpp"
#include "entity_state.hpp"
#include "game_state.hpp"

namespace te {

void inputEntity(entitystate_t& state, const Uint8 *keyboard, const gamestate_t& game) {
	static constexpr float SPEED = 3 * 32.0f;

	glm::vec3& playerVelocity = state.velocities[game.world.playerEntity];
	playerVelocity.x = (keyboard[SDL_SCANCODE_D] - keyboard[SDL_SCANCODE_A]) * SPEED;
	if (!keyboard[SDL_SCANCODE_SPACE] && playerVelocity.y < 0) {
		playerVelocity.y = 0;
	}
}

}
