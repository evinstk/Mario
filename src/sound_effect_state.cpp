#include "sound_effect_state.hpp"
#include "world_state.hpp"
#include "game_state.hpp"
#include "util.hpp"

namespace te {

static soundeffectstate_t gState;
const soundeffectstate_t& gSoundEffect = gState;

void runMusicCommandQueue(levelid_t levelID,
								 const gamestate_t& game) {
	auto musicIt = game.level.music.find(levelID);
	if (musicIt != game.level.music.end()) {
		gState.musicCommandQueue.push_back({ musiccmd_t::PLAY, musicIt->second });
	}
}

void stepMusicCommandQueue() {
	if (gWorld.deathTrigger) {
		gState.musicCommandQueue.push_back({ musiccmd_t::PAUSE, {} });
	}
}

void flushMusicCommandQueue() {
	gState.musicCommandQueue.clear();
}

} // namespace te
