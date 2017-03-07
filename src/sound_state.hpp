#pragma once
#include "types.hpp"
#include <SDL.h>
#include <SDL_mixer.h>
#include <memory>

namespace te {

struct soundstate_t {
	soundstate_t()
		: nextSoundID(1)
		, nextMusicID(1)
		{}

	int nextSoundID;
	stringmap_t<soundid_t> soundID;
	soundmap_t<chunkptr_t> chunk;

	int nextMusicID;
	stringmap_t<musicid_t> musicID;
	musicmap_t<musicptr_t> music;

	void loadSound(chunkptr_t&& chunk, const char *pathname);
	void loadMusic(musicptr_t&& pMusic, const char *pathname);
};

} // namespace te
