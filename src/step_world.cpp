#include "reducer_world.hpp"
#include "world_state.hpp"
#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include <EASTL/algorithm.h>
#include <EASTL/iterator.h>
#include <glm/gtx/transform.hpp>
#include <SDL.h>

namespace te {

static void stepView(const glm::vec3& playerTranslation,
					 glm::mat4& view) {
	view = glm::translate(glm::ivec3(-playerTranslation.x,
									 -playerTranslation.y,
									 0));
}

void stepWorld(worldstate_t& state, float dt, const gamestate_t& gameState) {
	stepEntity(state.entity, dt, gameState);
	stepView(state.entity.translations[state.playerEntity], state.view);
}

} // namespace te
