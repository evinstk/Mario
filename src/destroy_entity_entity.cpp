#include "game_action.hpp"
#include "entity_state.hpp"
#include "game_state.hpp"
#include <EASTL/algorithm.h>
#include <limits>

namespace te {

static void destroyEntityTranslations(entitymap_t<glm::vec3>& state, const gamestate_t& game) {
	for (entityid_t entityID : game.world.destroyQueue) {
		glm::vec3& t = state[entityID];
		t.x = t.y = std::numeric_limits<float>::min();
	}
}

static void destroyEntityLifetimes(entitymap_t<float>& state, const gamestate_t& game) {
	auto rowIt = state.begin();
	while (rowIt != state.end()) {
		if (game.world.destroyQueue.find(rowIt->first) != game.world.destroyQueue.end()) {
			rowIt = state.erase(rowIt);
		} else {
			++rowIt;
		}
	}
}

void destroyEntity(entitystate_t& state, const gamestate_t& game) {
	destroyEntityTranslations(state.translations, game);
	destroyEntityLifetimes(state.lifetimes, game);
}

} // namespace te
