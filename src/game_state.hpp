#pragma once
#include "tileset_state.hpp"
#include "level_state.hpp"
#include "world_state.hpp"

namespace te {

struct gamestate_t {
	tilesetstate_t tileset;
	levelstate_t level;
	worldstate_t world;
};

} // namespace te
