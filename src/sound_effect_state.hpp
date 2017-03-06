#pragma once
#include "types.hpp"
#include <EASTL/vector.h>

namespace te {

struct gamestate_t;

struct soundeffectstate_t {
	eastl::vector<eastl::pair<musiccmd_t, musicid_t>> musicCommandQueue;

	void run(levelid_t levelID);
	void step();
	void flush();

	const gamestate_t& game;
	soundeffectstate_t(const gamestate_t& g) : game(g) {}

private:
	void runMusicCommandQueue(eastl::vector<eastl::pair<musiccmd_t, musicid_t>>& state, levelid_t levelID) const;
	void stepMusicCommandQueue(eastl::vector<eastl::pair<musiccmd_t, musicid_t>>& state) const;
	void flushMusicCommandQueue(eastl::vector<eastl::pair<musiccmd_t, musicid_t>>& state) const;
};

} // namespace te
