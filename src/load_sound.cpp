#include "game_action.hpp"
#include "sound_state.hpp"

namespace te {

void soundstate_t::loadSound(chunkptr_t&& pChunk, const char *pathname) {
	if (soundID.find_as(pathname) != soundID.end()) {
		return;
	}

	soundid_t id(nextSoundID++);
	soundID.insert(decltype(soundstate_t::soundID)::value_type{ pathname, id });
	decltype(soundstate_t::chunk)::value_type newRow(id, eastl::move(pChunk));
	chunk.insert(eastl::move(newRow));
}

void soundstate_t::loadMusic(musicptr_t&& pMusic, const char *pathname) {
	if (musicID.find_as(pathname) != musicID.end()) {
		return;
	}

	musicid_t id(nextMusicID++);
	musicID.insert(decltype(musicID)::value_type{ pathname, id });
	decltype(soundstate_t::music)::value_type newRow(id, eastl::move(pMusic));
	music.insert(eastl::move(newRow));
}

} // namespace te
