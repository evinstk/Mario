#include "game_action.hpp"
#include "game_state.hpp"

namespace te {

void stepGame(gamestate_t& state, float dt) {
	stepWorld(state.world, dt, state);
	state.soundEffect.step();
}

} // namespace te
