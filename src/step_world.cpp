#include "reducer_world.hpp"
#include "world_state.hpp"
#include "util.hpp"
#include "camera.hpp"
#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include <EASTL/algorithm.h>
#include <EASTL/iterator.h>
#include <SDL.h>

#include <iostream>

namespace te {

static void stepView(glm::mat4& state,
					 float dt,
					 const gamestate_t& game) {
	static constexpr int HALF_CAMERA = (CAMERA_WIDTH / 2) - 16;
	static constexpr int PUSH_THRESHOLD = HALF_CAMERA - 32;

	glm::vec3 playerTranslation = game.world.entity.translations.find(game.world.playerEntity)->second;
	glm::vec3 playerView = state * glm::vec4(playerTranslation, 1.0f);
	float xPlayerVelocity = game.world.entity.velocities.find(game.world.playerEntity)->second.x;

	if (playerView.x > HALF_CAMERA) {
		state = glm::translate(state, glm::vec3(HALF_CAMERA - playerView.x, 0, 0));
	} else if (playerView.x > PUSH_THRESHOLD && xPlayerVelocity > 0) {
		float pushFactor = (playerView.x / HALF_CAMERA) * 0.75f;
		state = glm::translate(state, glm::vec3(-xPlayerVelocity * dt * pushFactor, 0, 0));
	}
}

void stepWorld(worldstate_t& state, float dt, const gamestate_t& gameState) {
	stepEntity(state.entity, dt, gameState);
	stepView(state.view, dt, gameState);
}

} // namespace te
