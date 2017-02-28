#include "game_action.hpp"
#include "game_state.hpp"
#include "util.hpp"

namespace te {

void runMusicCommandQueue(levelid_t levelID, const gamestate_t& game);

void runGame(gamestate_t& state, levelid_t levelID) {
	runWorld(levelID, state.level);
	runMusicCommandQueue(levelID, state);
}

} // namespace te
