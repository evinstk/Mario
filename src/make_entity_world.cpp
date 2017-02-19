#include "game_action.hpp"
#include "world_state.hpp"

namespace te {

void makeEntity(worldstate_t& state,
				const gamestate_t& game) {
	state.newEntityIDs.clear();
	for (auto it = state.newEntityQueue.begin(), end = state.newEntityQueue.end(); it < end; ++it) {
		state.newEntityIDs.push_back(entityid_t(state.nextEntityID++));
	}
	makeEntity(state.entity, game);
	state.newEntityQueue.clear();
	state.newEntityIDs.clear();
}

} // namespace te
