#include "game_action.hpp"
#include "world_state.hpp"
#include "game_state.hpp"

namespace te {

static void destroyEntityFreeIDs(entityset_t& state, const gamestate_t& game) {
	for (entityid_t freeID : gWorld.destroyQueue) {
		state.insert(freeID);
	}
}

void destroyEntityWorld(worldstate_t& state,
						const gamestate_t& game) {
	destroyEntity();
	destroyEntityFreeIDs(state.freeIDs, game);
	state.destroyQueue.clear();
}

} // namespace te
