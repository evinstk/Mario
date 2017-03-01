#include "game_action.hpp"
#include "sound_state.hpp"

namespace te {

void loadSound(soundstate_t& state, chunkptr_t&& chunk, const char *pathname) {
	if (state.soundID.find_as(pathname) != state.soundID.end()) {
		return;
	}

	soundid_t soundID(state.nextSoundID++);
	state.soundID.insert(decltype(soundstate_t::soundID)::value_type{ pathname, soundID });
	decltype(soundstate_t::chunk)::value_type newRow(soundID, eastl::move(chunk));
	state.chunk.insert(eastl::move(newRow));
}

void loadMusic(soundstate_t& state, musicptr_t&& pMusic, const char *pathname) {
	if (state.musicID.find_as(pathname) != state.musicID.end()) {
		return;
	}

	musicid_t musicID(state.nextMusicID++);
	state.musicID.insert(decltype(state.musicID)::value_type{ pathname, musicID });
	decltype(soundstate_t::music)::value_type newRow(musicID, eastl::move(pMusic));
	state.music.insert(eastl::move(newRow));
}

} // namespace te
