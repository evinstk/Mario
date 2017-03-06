#include "game_action.hpp"
#include "game_state.hpp"

namespace te {

void destroyEntity(gamestate_t& state) {
	destroyEntity(state.world, state);
}

} // namespace te
