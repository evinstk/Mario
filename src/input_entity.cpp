#include "game_action.hpp"
#include "entity_state.hpp"
#include "game_state.hpp"

namespace te {

void entitystate_t::input(const Uint8 *keyboard) {
	static constexpr float SPEED = 3 * 32.0f;

	if (pGame->world.mode != worldmode_t::PLAY) {
		return;
	}

	glm::vec3& playerVelocity = velocities[pGame->world.playerEntity];
	playerVelocity.x = (keyboard[SDL_SCANCODE_D] - keyboard[SDL_SCANCODE_A]) * SPEED;
	if (!keyboard[SDL_SCANCODE_SPACE] && playerVelocity.y < 0) {
		playerVelocity.y = 0;
	}

	if (pGame->world.deathTrigger) {
		playerVelocity.x = 0;
	}
}

}
