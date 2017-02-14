#include "reducer_world.hpp"
#include "game_state.hpp"

namespace te {

void inputGame(gamestate_t& state, const SDL_Event& evt) {
	inputWorld(state.world, evt, state);
}

} // namespace te
