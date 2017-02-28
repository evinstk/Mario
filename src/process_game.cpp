#include "game_action.hpp"
#include "game_state.hpp"

namespace te {

void processGame(gamestate_t& state, const SDL_Event& evt) {
	processWorld(evt, state);
}

} // namespace te
