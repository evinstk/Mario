#include "sound_state.hpp"

namespace te {

static soundstate_t gState;
const soundstate_t& gSound = gState;

void loadSound(soundstate_t& state, chunkptr_t&& chunk, const char *pathname);
void loadSound(chunkptr_t&& chunk, const char *pathname) {
	loadSound(gState, eastl::move(chunk), pathname);
}

void loadMusic(soundstate_t& state, musicptr_t&& pMusic, const char *pathname);
void loadMusic(musicptr_t&& pMusic, const char *pathname) {
	loadMusic(gState, eastl::move(pMusic), pathname);
}

} // namespace te
