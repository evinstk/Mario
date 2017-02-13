#pragma once
#include "types.hpp"

namespace te {

struct levelstate_t {
	levelstate_t() : nextLevelID(1) {}

	int nextLevelID;
	stringmap_t<levelid_t> source;

	levelmap_t<map_t> map;
	levelmap_t<vector_t<leveltileset_t>> tilesets;
	levelmap_t<vector_t<layer_t>> layers;

	levelobjectmap_t<levelobject_t> objects;
	levelmap_t<int> playerObject;
};

} // namespace te
