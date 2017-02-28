#include "game_action.hpp"
#include "world_state.hpp"

namespace te {

void inputWorld(worldstate_t& state, const Uint8 *keyboardState, const gamestate_t& game) {
	inputEntity(keyboardState, game);
}

} // namespace te
