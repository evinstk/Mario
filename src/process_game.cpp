#include "reducer_world.hpp"
#include "game_state.hpp"

namespace te {

void processGame(gamestate_t& state, const SDL_Event& evt) {
	processWorld(state.world, evt, state);
}

} // namespace te
