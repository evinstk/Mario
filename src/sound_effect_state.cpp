#include "sound_effect_state.hpp"
#include "game_state.hpp"
#include "util.hpp"
#include "game_values.hpp"

namespace te {

void soundeffectstate_t::runMusicCommandQueue(eastl::vector<eastl::pair<musiccmd_t, musicid_t>>& state,
											  levelid_t levelID) const {
	auto musicIt = game.level.music.find(levelID);
	if (musicIt != game.level.music.end()) {
		Mix_Music *pMusic = getMusic(musicIt->second, game);
		Mix_PlayMusic(pMusic, -1);
	}
}

void soundeffectstate_t::stepSoundQueue(eastl::vector_set<soundid_t>& state) const {
	for (entityid_t entityID : game.world.entity.hitGround) {
		auto soundIt = game.world.entity.bounceSounds.find(entityID);
		if (soundIt != game.world.entity.bounceSounds.end() && canBounce(entityID, game)) {
			state.insert(soundIt->second);
		}
	}

	for (entityrequest_t newEntity : game.world.newEntityQueue) {
		if (newEntity.type == entity_t::BLOCK_COIN) {
			state.insert(getSoundID(COIN_SOUND, game));
		}
	}

	if (game.world.deathTrigger) {
		auto soundIt = game.level.dieMusic.find(game.world.level);
		if (soundIt != game.level.dieMusic.end()) {
			state.insert(soundIt->second);
		}
	}
}

void soundeffectstate_t::stepMusicCommandQueue(eastl::vector<eastl::pair<musiccmd_t, musicid_t>>& state) const {
	if (game.world.deathTrigger) {
		state.push_back({ musiccmd_t::PAUSE, {} });
	}
}

void soundeffectstate_t::run(levelid_t levelID) {
	runMusicCommandQueue(musicCommandQueue, levelID);
}

void soundeffectstate_t::dispatch() {
	for (playercmd_t cmd : game.command.commandQueue) {
		if (cmd == playercmd_t::JUMP) {
			soundQueue.push_back(getSoundID(JUMP_SOUND, game));
		}
	}
}

void soundeffectstate_t::step() {
	stepSoundQueue(soundQueue);
	stepMusicCommandQueue(musicCommandQueue);
}

void soundeffectstate_t::flush() {
	soundQueue.clear();
	musicCommandQueue.clear();
}

} // namespace te
