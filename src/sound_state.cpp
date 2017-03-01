#include "sound_state.hpp"

namespace te {

static soundstate_t gState;
const soundstate_t& gSound = gState;

void loadSound(soundstate_t& state, std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>&& chunk, const char *pathname);
void loadSound(std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>&& chunk, const char *pathname) {
	loadSound(gState, std::move(chunk), pathname);
}

void loadMusic(soundstate_t& state, musicptr_t&& pMusic, const char *pathname);
void loadMusic(musicptr_t&& pMusic, const char *pathname) {
	loadMusic(gState, std::move(pMusic), pathname);
}

} // namespace te
