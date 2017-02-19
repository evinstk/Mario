#include "game_action.hpp"
#include "game_state.hpp"
#include "util.hpp"
#include <glm/glm.hpp>
#include <cassert>

namespace te {

static void runPlayerEntity(entityid_t& playerEntity, levelid_t levelID, const levelmap_t<int>& playerObjectState) {
	auto playerEntityIt = playerObjectState.find(levelID);
	assert(playerEntityIt != playerObjectState.end());
	playerEntity = entityid_t(playerEntityIt->second);
}

static void runView(glm::imat4& view,
					const glm::vec3& playerTranslation) {
	setView(view, playerTranslation);
}

void runWorld(worldstate_t& state, levelid_t levelID, const levelstate_t& levelState) {
	// clear world state on run
	state = worldstate_t();

	state.level = levelID;
	auto layersIt = levelState.layers.find(levelID);
	assert(layersIt != levelState.layers.end());
	state.layers = layersIt->second;

	runEntity(state.entity, levelID, levelState.objects);
	runPlayerEntity(state.playerEntity, levelID, levelState.playerObject);
	runView(state.view, state.entity.translations[state.playerEntity]);
}

}
