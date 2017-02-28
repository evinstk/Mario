#include "game_action.hpp"
#include "game_state.hpp"

namespace te {

void destroyEntity(gamestate_t& state) {
	destroyEntityWorld(state);
}

} // namespace te
