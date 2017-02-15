#include "reducer_world.hpp"
#include "game_state.hpp"

namespace te {

void stepGame(gamestate_t& state, float dt) {
	stepWorld(state.world, dt, state);
}

} // namespace te
