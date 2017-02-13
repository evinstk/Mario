#include "reducer_world.hpp"
#include "game_state.hpp"

namespace te {

void stepGame(gamestate_t& state, float dt, const Uint8 *keyboardState) {
	stepWorld(state.world, dt, keyboardState, state.tileset, state.level);
}

} // namespace te
