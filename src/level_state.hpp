#pragma once
#include "level_object_state.hpp"
#include "types.hpp"

namespace te {

struct levelstate_t {
	levelstate_t() : nextLevelID(1) {}

	int nextLevelID;
	stringmap_t<levelid_t> source;

	levelmap_t<map_t> map;
	levelmap_t<vector_t<leveltileset_t>> tilesets;
	levelmap_t<vector_t<layer_t>> layers;
	levelmap_t<int> platformIndex;

	levelobjectmap_t<levelobject_t> objects;

	levelobjectstate_t objects2;

	levelmap_t<int> playerObject;
};

} // namespace te
