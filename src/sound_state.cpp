#include "sound_state.hpp"
#include <EASTL/unique_ptr.h>

namespace te {

static eastl::unique_ptr<soundstate_t> gpState{};
const soundstate_t *gSound = nullptr;

void initSoundState() {
	gpState = eastl::make_unique<soundstate_t>();
	gSound = gpState.get();
}

void quitSoundState() {
	gpState.reset();
	gSound = nullptr;
}

void loadSound(soundstate_t& state, chunkptr_t&& chunk, const char *pathname);
void loadSound(chunkptr_t&& chunk, const char *pathname) {
	loadSound(*gpState, eastl::move(chunk), pathname);
}

void loadMusic(soundstate_t& state, musicptr_t&& pMusic, const char *pathname);
void loadMusic(musicptr_t&& pMusic, const char *pathname) {
	loadMusic(*gpState, eastl::move(pMusic), pathname);
}

} // namespace te
