#include "game_action.hpp"
#include "game_state.hpp"

namespace te {

void makeEntity(gamestate_t& state) {
	makeEntity(state.world, state);
}

} // namespace te
