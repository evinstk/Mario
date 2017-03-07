#include "game_action.hpp"
#include "game_state.hpp"
#include "util.hpp"

namespace te {

void runGame(gamestate_t& state, levelid_t levelID) {
	state.world.run(levelID);
	state.soundEffect.run(levelID);
}

} // namespace te
