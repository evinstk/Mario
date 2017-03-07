#pragma once
#include "types.hpp"
#include <EASTL/vector.h>
#include <EASTL/vector_set.h>

namespace te {

struct gamestate_t;

struct soundeffectstate_t {
	eastl::vector<eastl::pair<musiccmd_t, musicid_t>> musicCommandQueue;
	eastl::vector_set<soundid_t> soundQueue;

	void run(levelid_t levelID);
	void dispatch();
	void step();
	void flush();

	soundeffectstate_t(const gamestate_t& g) : game(g) {}

private:
	const gamestate_t& game;

	void runMusicCommandQueue(eastl::vector<eastl::pair<musiccmd_t, musicid_t>>& state, levelid_t levelID) const;
	void stepSoundQueue(eastl::vector_set<soundid_t>& state) const;
	void stepMusicCommandQueue(eastl::vector<eastl::pair<musiccmd_t, musicid_t>>& state) const;
};

} // namespace te
