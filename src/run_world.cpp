#include "game_action.hpp"
#include "game_state.hpp"
#include "util.hpp"
#include <glm/glm.hpp>
#include <cassert>

namespace te {

void worldstate_t::runNextEntityID(int& state, levelid_t levelID) const {
	state = pGame->level.nextObjectID.find(levelID)->second;
}

void worldstate_t::runPlayerEntity(entityid_t& playerEntity, levelid_t levelID) const {
	auto playerEntityIt = pGame->level.playerObject.find(levelID);
	assert(playerEntityIt != pGame->level.playerObject.end());
	playerEntity = entityid_t(playerEntityIt->second);
}

static void runView(glm::imat4& view,
					const glm::vec3& playerTranslation) {
	setView(view, playerTranslation);
}

void worldstate_t::run(levelid_t levelID) {
	// clear world state on run
	*this = worldstate_t(*pGame);

	level = levelID;
	auto layersIt = pGame->level.layers.find(levelID);
	assert(layersIt != pGame->level.layers.end());
	layers = layersIt->second;

	runEntity(entity, levelID, pGame->level.objects);
	runNextEntityID(nextEntityID, levelID);
	runPlayerEntity(playerEntity, levelID);
	runView(view, entity.translations[playerEntity]);
}

}
