#include "sound_effect_state.hpp"
#include "game_state.hpp"
#include "util.hpp"

namespace te {

void soundeffectstate_t::runMusicCommandQueue(eastl::vector<eastl::pair<musiccmd_t, musicid_t>>& state,
											  levelid_t levelID) const {
	auto musicIt = game.level.music.find(levelID);
	if (musicIt != game.level.music.end()) {
		Mix_Music *pMusic = getMusic(musicIt->second, game);
		Mix_PlayMusic(pMusic, -1);
	}
}

void soundeffectstate_t::stepMusicCommandQueue(eastl::vector<eastl::pair<musiccmd_t, musicid_t>>& state) const {
	if (game.world.deathTrigger) {
		state.push_back({ musiccmd_t::PAUSE, {} });
	}
}

void soundeffectstate_t::flushMusicCommandQueue(eastl::vector<eastl::pair<musiccmd_t, musicid_t>>& state) const {
	state.clear();
}

void soundeffectstate_t::run(levelid_t levelID) {
	runMusicCommandQueue(musicCommandQueue, levelID);
}

void soundeffectstate_t::step() {
	stepMusicCommandQueue(musicCommandQueue);
}

void soundeffectstate_t::flush() {
	flushMusicCommandQueue(musicCommandQueue);
}

} // namespace te
