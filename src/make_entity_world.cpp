#include "game_action.hpp"
#include "world_state.hpp"

namespace te {

void makeEntity(worldstate_t& state,
				const gamestate_t& game) {
	state.newEntityIDs.clear();
	for (auto it = state.newEntityQueue.begin(), end = state.newEntityQueue.end(); it < end; ++it) {
		entityid_t newID;
		auto freeIDIt = state.freeIDs.begin();
		if (freeIDIt != state.freeIDs.end()) {
			newID = entityid_t(*freeIDIt);
			state.freeIDs.erase(freeIDIt);
		} else {
			newID = entityid_t(state.nextEntityID++);
		}
		state.newEntityIDs.push_back(newID);
	}
	makeEntity(state.entity, game);
	state.newEntityQueue.clear();
	state.newEntityIDs.clear();
}

} // namespace te
