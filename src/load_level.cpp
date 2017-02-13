#include "reducer_world.hpp"
#include "level_state.hpp"
#include <EASTL/algorithm.h>
#include <cassert>

namespace te {

static bool loadSource(stringmap_t<levelid_t>& state, int& nextID, const char *pathname) {
	auto existingIt = state.find_as(pathname);
	if (existingIt != state.end()) {
		return false;
	}

	levelid_t levelID(nextID++);
	state.insert({ eastl::string(pathname), levelID });
	return true;
}

static void loadMap(levelmap_t<map_t>& state,
					const tmx_t& tmx,
					levelid_t levelID) {

	map_t map = {
		.size = { tmx.width, tmx.height },
		.tileSize = { tmx.tilewidth, tmx.tileheight }
	};
	state.insert({ levelID, map });
}

static void loadLayers(levelmap_t<vector_t<layer_t>>& state,
					   const decltype(tmx_t::layers)& tmxLayers,
					   levelid_t levelID) {

	auto& layers = state[levelID];
	eastl::transform(tmxLayers.begin(), tmxLayers.end(), eastl::back_inserter(layers), [](const tmxlayer_t& tmxlayer) {

			layer_t layer = {
				.gids = {},
				.layerIndex = tmxlayer.layerIndex,
				.size = glm::ivec2(tmxlayer.width, tmxlayer.height)
			};

			eastl::transform(tmxlayer.data.tiles.begin(),
							 tmxlayer.data.tiles.end(),
							 eastl::back_inserter(layer.gids),
							 [](tmxtile_t tile) {
								 return tile.gid;
							 });

			return layer;
		});
}

static void loadTilesets(levelmap_t<vector_t<leveltileset_t>>& state,
						 const decltype(tmx_t::externalTilesets)& externalTilesets,
						 levelid_t levelID,
						 const stringmap_t<tilesetid_t>& tilesetIDs) {

	auto& layerTilesets = state[levelID];
	layerTilesets.clear();
	for (const auto& tileset : externalTilesets) {
		auto tilesetIDIter = tilesetIDs.find_as(tileset.source.c_str());
		assert(tilesetIDIter != tilesetIDs.end());

		tilesetid_t tilesetID = tilesetIDIter->second;
		leveltileset_t levelTileset = {
			.tileset = tilesetID,
			.firstgid = tileset.firstgid
		};
		layerTilesets.push_back(levelTileset);
	}
}

void loadLevel(levelstate_t& state, const tmx_t& tmx, const char *pathname, const stringmap_t<tilesetid_t>& tilesetIDs) {
	bool loaded = loadSource(state.source, state.nextLevelID, pathname);
	if (!loaded) {
		return;
	}

	levelid_t levelID = state.source.find_as(pathname)->second;
	loadMap(state.map, tmx, levelID);
	loadTilesets(state.tilesets, tmx.externalTilesets, levelID, tilesetIDs);
	loadLayers(state.layers, tmx.layers, levelID);
}

} // namespace te
