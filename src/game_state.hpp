#pragma once
#include "sound_state.hpp"
#include "level_state.hpp"

namespace te {

struct gamestate_t {
	soundstate_t sound;
	levelstate_t level;
};

} // namespace te
