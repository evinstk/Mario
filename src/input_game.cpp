#include "reducer_world.hpp"
#include "game_state.hpp"

namespace te {

void inputGame(gamestate_t& state, const Uint8 *keyboardState) {
	inputWorld(state.world, keyboardState, state);
}

} // namespace te
