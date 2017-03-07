#pragma once
#include "level_object_state.hpp"
#include "types.hpp"

namespace te {

struct gamestate_t;
struct tmx_t;

struct levelstate_t {
	levelstate_t(const gamestate_t& g) : nextLevelID(1), objects(g), pGame(&g) {}

	int nextLevelID;
	stringmap_t<levelid_t> source;

	levelmap_t<map_t> map;
	levelmap_t<vector_t<leveltileset_t>> tilesets;
	levelmap_t<vector_t<layer_t>> layers;
	levelmap_t<int> platformIndex;

	levelobjectstate_t objects;

	levelmap_t<int> nextObjectID;
	levelmap_t<int> playerObject;

	levelmap_t<musicid_t> music;
	levelmap_t<soundid_t> dieMusic;

	void load(const tmx_t& tmx, const char *pathname);

private:
	const gamestate_t *pGame;
};

} // namespace te
