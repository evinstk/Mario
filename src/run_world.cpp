#include "reducer_world.hpp"
#include "game_state.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <cassert>

namespace te {

static void runPlayerEntity(entity_t& playerEntity, levelid_t levelID, const levelmap_t<int>& playerObjectState) {
	auto playerEntityIt = playerObjectState.find(levelID);
	assert(playerEntityIt != playerObjectState.end());
	playerEntity = entity_t(playerEntityIt->second);
}

static void runView(glm::mat4& view,
					const glm::vec3& playerTranslation) {
	view = glm::translate(glm::ivec3(-playerTranslation.x,
									 -playerTranslation.y,
									 0));
}

void runWorld(worldstate_t& state, levelid_t levelID, const levelstate_t& levelState) {
	// clear world state on run
	state = worldstate_t();

	state.level = levelID;
	auto layersIt = levelState.layers.find(levelID);
	assert(layersIt != levelState.layers.end());
	state.layers = layersIt->second;

	runEntity(state.entity, levelID, levelState);
	runPlayerEntity(state.playerEntity, levelID, levelState.playerObject);
	runView(state.view, state.entity.translations[state.playerEntity]);
}

}
