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
					 const glm::vec3& playerTranslation) {
	static constexpr int HALF_CAMERA = (CAMERA_WIDTH / 2) - 16;

	const glm::vec3 playerView = state * glm::vec4(playerTranslation, 1.0f);
	if (playerView.x > HALF_CAMERA) {
		state = glm::translate(state, glm::vec3(HALF_CAMERA - playerView.x, 0, 0));
	}
}

void stepWorld(worldstate_t& state, float dt, const gamestate_t& gameState) {
	stepEntity(state.entity, dt, gameState);
	stepView(state.view, state.entity.translations[state.playerEntity]);
}

} // namespace te
