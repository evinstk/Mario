#include "game_action.hpp"
#include "game_state.hpp"
#include "world_state.hpp"

namespace te {

static void stepMusicCommandQueue(eastl::vector<std::pair<musiccmd_t, musicid_t>>& state,
								  const gamestate_t& game) {
	if (gWorld.deathTrigger) {
		state.push_back({ musiccmd_t::PAUSE, {} });
	}
}

void flushMusicCommandQueue(gamestate_t& state) {
	state.musicCommandQueue.clear();
}

void stepGame(gamestate_t& state, float dt) {
	stepWorld(dt, state);
	stepMusicCommandQueue(state.musicCommandQueue, state);
}

} // namespace te
