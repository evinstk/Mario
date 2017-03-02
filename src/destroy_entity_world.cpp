#include "game_action.hpp"
#include "world_state.hpp"

namespace te {

static void destroyEntityFreeIDs(entityset_t& state) {
	for (entityid_t freeID : gWorld.destroyQueue) {
		state.insert(freeID);
	}
}

void destroyEntityWorld(worldstate_t& state) {
	destroyEntityEntity();
	destroyEntityFreeIDs(state.freeIDs);
	state.destroyQueue.clear();
}

} // namespace te
