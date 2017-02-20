#pragma once
#include "types.hpp"
#include <SDL.h>
#include <SDL_mixer.h>
#include <memory>

namespace te {

struct soundstate_t {
	soundstate_t() : nextSoundID(1) {}

	int nextSoundID;
	stringmap_t<soundid_t> soundID;
	soundmap_t<std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>> chunk;
};

} // namespace te
