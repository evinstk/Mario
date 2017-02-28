#include "game_action.hpp"
#include "world_state.hpp"
#include "game_state.hpp"
#include "game_values.hpp"
#include "util.hpp"
#include <SDL.h>

namespace te {

void processWorld(worldstate_t& state, const SDL_Event& evt, const gamestate_t& game) {
	bool jump = false;
	processEntity(evt, state.playerEntity, game, jump);
	if (jump) {
		state.soundQueue.push_back(getSoundID(JUMP_SOUND, game));
	}
}

} // namespace te
