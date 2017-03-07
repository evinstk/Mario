#include "game_action.hpp"
#include "world_state.hpp"
#include "game_state.hpp"

namespace te {

void worldstate_t::destroyEntityFreeIDs(entityset_t& state) const {
	for (entityid_t freeID : pGame->world.destroyQueue) {
		state.insert(freeID);
	}
}

void worldstate_t::destroyEntity() {
	te::destroyEntity(entity, *pGame);
	destroyEntityFreeIDs(freeIDs);
	destroyQueue.clear();
}

} // namespace te
