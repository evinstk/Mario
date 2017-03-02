#include "game_action.hpp"
#include "world_state.hpp"

namespace te {

void inputWorld(worldstate_t& state, const Uint8 *keyboardState) {
	inputEntity(keyboardState);
}

} // namespace te
