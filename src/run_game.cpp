#include "game_action.hpp"
#include "game_state.hpp"
#include "util.hpp"

namespace te {

static void runMusicCommandQueue(eastl::vector<std::pair<musiccmd_t, musicid_t>>& state,
								 levelid_t levelID,
								 const gamestate_t& game) {
	auto musicIt = game.level.music.find(levelID);
	if (musicIt != game.level.music.end()) {
		Mix_Music *pMusic = getMusic(musicIt->second, game);
		Mix_PlayMusic(pMusic, -1);
	}
}

void runGame(gamestate_t& state, levelid_t levelID) {
	runWorld(state.world, levelID, state.level);
	runMusicCommandQueue(state.musicCommandQueue, levelID, state);
}

} // namespace te
