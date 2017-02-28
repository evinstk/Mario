#pragma once
#include "tileset_state.hpp"
#include "sound_state.hpp"
#include "level_state.hpp"

namespace te {

struct gamestate_t {
	tilesetstate_t tileset;
	soundstate_t sound;
	levelstate_t level;
};

} // namespace te
