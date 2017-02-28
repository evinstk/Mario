#include "game_action.hpp"
#include "game_state.hpp"
#include "world_state.hpp"

namespace te {

void stepMusicCommandQueue();

void stepGame(gamestate_t& state, float dt) {
	stepWorld(dt, state);
	stepMusicCommandQueue();
}

} // namespace te
