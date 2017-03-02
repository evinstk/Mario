#include "game_action.hpp"
#include "entity_state.hpp"
#include "world_state.hpp"

namespace te {

void inputEntity(entitystate_t& state, const Uint8 *keyboard) {
	static constexpr float SPEED = 3 * 32.0f;

	if (gWorld.mode != worldmode_t::PLAY) {
		return;
	}

	glm::vec3& playerVelocity = state.velocities[gWorld.playerEntity];
	playerVelocity.x = (keyboard[SDL_SCANCODE_D] - keyboard[SDL_SCANCODE_A]) * SPEED;
	if (!keyboard[SDL_SCANCODE_SPACE] && playerVelocity.y < 0) {
		playerVelocity.y = 0;
	}

	if (gWorld.deathTrigger) {
		playerVelocity.x = 0;
	}
}

}
