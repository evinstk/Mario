#include "game_action.hpp"
#include "world_state.hpp"

namespace te {

void worldstate_t::input(const Uint8 *keyboardState) {
	entity.input(keyboardState);
}

} // namespace te
