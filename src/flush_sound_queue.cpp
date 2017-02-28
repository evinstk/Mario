#include "game_action.hpp"
#include "world_state.hpp"

namespace te {

void flushSoundQueue(worldstate_t& state) {
	state.soundQueue.clear();
}

} // namespace te
