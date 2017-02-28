#pragma once
#include "tileset_state.hpp"
#include "sound_state.hpp"
#include "level_state.hpp"
#include "types.hpp"
#include <EASTL/vector.h>

namespace te {

struct gamestate_t {
	tilesetstate_t tileset;
	soundstate_t sound;
	levelstate_t level;

	eastl::vector<std::pair<musiccmd_t, musicid_t>> musicCommandQueue;
};

} // namespace te
