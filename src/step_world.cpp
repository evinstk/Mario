#include "reducer_world.hpp"
#include "world_state.hpp"
#include "util.hpp"
#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include <EASTL/algorithm.h>
#include <EASTL/iterator.h>
#include <SDL.h>

namespace te {

static void stepView(glm::mat4& state,
					 const glm::vec3& playerTranslation) {
	setView(state, playerTranslation);
}

void stepWorld(worldstate_t& state, float dt, const gamestate_t& gameState) {
	stepEntity(state.entity, dt, gameState);
	stepView(state.view, state.entity.translations[state.playerEntity]);
}

} // namespace te
