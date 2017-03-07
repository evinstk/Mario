#include "command_state.hpp"
#include "game_state.hpp"
#include <SDL.h>

namespace te {

void commandstate_t::process(const SDL_Event& evt) {
	if (game.world.mode != worldmode_t::PLAY) {
		return;
	}

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_SPACE &&
			evt.key.repeat == 0 &&
			game.world.entity.groundOffsets.find(game.world.playerEntity) != game.world.entity.groundOffsets.end()) {

			commandQueue.push_back(playercmd_t::JUMP);
		}
	}
}

void commandstate_t::flush() {
	commandQueue.clear();
}

} // namespace te
