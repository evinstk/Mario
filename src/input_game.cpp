#include "game_action.hpp"
#include "game_state.hpp"

namespace te {

void inputGame(gamestate_t& state, const Uint8 *keyboardState) {
	state.world.input(keyboardState);
}

} // namespace te
