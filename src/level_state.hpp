#pragma once
#include "types.hpp"

namespace te {

struct levelstate_t {
	levelstate_t() : nextLevelID(1) {}

	int nextLevelID;
	stringmap_t<levelid_t> source;

	levelmap_t<map_t> map;
	levelmap_t<vector_t<tilesetid_t>> tilesets;
	levelmap_t<vector_t<layer_t>> layers;
};

} // namespace te
