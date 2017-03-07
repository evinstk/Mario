#include "game_action.hpp"
#include "game_state.hpp"

namespace te {

void destroyEntity(gamestate_t& state) {
	state.world.destroyEntity();
}

} // namespace te
