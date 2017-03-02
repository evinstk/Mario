#include "game_action.hpp"
#include "game_state.hpp"
#include "util.hpp"
#include <glm/glm.hpp>
#include <cassert>

namespace te {

static void runNextEntityID(int& state, levelid_t levelID) {
	state = gLevel.nextObjectID.find(levelID)->second;
}

static void runPlayerEntity(entityid_t& playerEntity, levelid_t levelID, const levelmap_t<int>& playerObjectState) {
	auto playerEntityIt = playerObjectState.find(levelID);
	assert(playerEntityIt != playerObjectState.end());
	playerEntity = entityid_t(playerEntityIt->second);
}

static void runView(glm::imat4& view,
					const glm::vec3& playerTranslation) {
	setView(view, playerTranslation);
}

void runWorld(worldstate_t& state, levelid_t levelID) {
	// clear world state on run
	state = worldstate_t();

	state.level = levelID;
	auto layersIt = gLevel.layers.find(levelID);
	assert(layersIt != gLevel.layers.end());
	state.layers = layersIt->second;

	runEntity(levelID);
	runNextEntityID(state.nextEntityID, levelID);
	runPlayerEntity(state.playerEntity, levelID, gLevel.playerObject);
	runView(state.view, getTranslation(state.playerEntity));
}

}
