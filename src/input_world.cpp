#include "reducer_world.hpp"
#include "world_state.hpp"

namespace te {

void inputWorld(worldstate_t& state, const Uint8 *keyboardState, const gamestate_t& game) {
	inputEntity(state.entity, keyboardState, game);
}

} // namespace te
