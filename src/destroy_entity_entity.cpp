#include "game_action.hpp"
#include "entity_state.hpp"
#include "world_state.hpp"
#include <EASTL/algorithm.h>
#include <limits>

namespace te {

static void destroyEntityTranslations(entitymap_t<glm::vec3>& state) {
	for (entityid_t entityID : gWorld.destroyQueue) {
		glm::vec3& t = state[entityID];
		t.x = t.y = std::numeric_limits<float>::min();
	}
}

static void destroyEntityLifetimes(entitymap_t<float>& state) {
	auto rowIt = state.begin();
	while (rowIt != state.end()) {
		if (gWorld.destroyQueue.find(rowIt->first) != gWorld.destroyQueue.end()) {
			rowIt = state.erase(rowIt);
		} else {
			++rowIt;
		}
	}
}

void destroyEntity(entitystate_t& state) {
	destroyEntityTranslations(state.translations);
	destroyEntityLifetimes(state.lifetimes);
}

} // namespace te
