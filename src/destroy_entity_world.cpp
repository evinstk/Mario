#include "game_action.hpp"
#include "world_state.hpp"

namespace te {

void destroyEntity(worldstate_t& state,
				   const gamestate_t& game) {
	destroyEntity(state.entity, game);
	state.destroyQueue.clear();
}

} // namespace te
