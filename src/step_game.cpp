#include "game_action.hpp"
#include "world_state.hpp"

namespace te {

void stepMusicCommandQueue();

void stepGame(float dt) {
	stepWorld(dt);
	stepMusicCommandQueue();
}

} // namespace te
