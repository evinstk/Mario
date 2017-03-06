#include "game_action.hpp"
#include "world_state.hpp"
#include "game_state.hpp"

namespace te {

static void destroyEntityFreeIDs(entityset_t& state, const gamestate_t& game) {
	for (entityid_t freeID : game.world.destroyQueue) {
		state.insert(freeID);
	}
}

void destroyEntity(worldstate_t& state,
				   const gamestate_t& game) {
	destroyEntity(state.entity, game);
	destroyEntityFreeIDs(state.freeIDs, game);
	state.destroyQueue.clear();
}

} // namespace te
