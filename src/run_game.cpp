#include "game_action.hpp"
#include "game_state.hpp"

namespace te {

void runGame(gamestate_t& state, levelid_t levelID) {
	runWorld(state.world, levelID, state.level);
}

}
