#include "game_action.hpp"
#include "game_state.hpp"

namespace te {

void stepGame(gamestate_t& state, float dt) {
	state.world.step(dt);
	state.soundEffect.step();
}

} // namespace te
