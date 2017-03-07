#include "game_action.hpp"
#include "entity_state.hpp"
#include "game_state.hpp"
#include <EASTL/algorithm.h>
#include <limits>

namespace te {

void entitystate_t::destroyEntityTranslations(entitymap_t<glm::vec3>& state) const {
	for (entityid_t entityID : pGame->world.destroyQueue) {
		glm::vec3& t = state[entityID];
		t.x = t.y = std::numeric_limits<float>::min();
	}
}

void entitystate_t::destroyEntityLifetimes(entitymap_t<float>& state) const {
	auto rowIt = state.begin();
	while (rowIt != state.end()) {
		if (pGame->world.destroyQueue.find(rowIt->first) != pGame->world.destroyQueue.end()) {
			rowIt = state.erase(rowIt);
		} else {
			++rowIt;
		}
	}
}

void entitystate_t::destroyEntity() {
	destroyEntityTranslations(translations);
	destroyEntityLifetimes(lifetimes);
}

} // namespace te
