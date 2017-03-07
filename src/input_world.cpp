#include "game_action.hpp"
#include "world_state.hpp"

namespace te {

void worldstate_t::input(const Uint8 *keyboardState) {
	inputEntity(entity, keyboardState, *pGame);
}

} // namespace te
