#include "game_action.hpp"
#include "game_state.hpp"

namespace te {

void flushSoundQueue(gamestate_t& state) {
	state.world.soundQueue.clear();
}

} // namespace te
