#include "reducer_world.hpp"
#include "level_state.hpp"
#include "tileset_state.hpp"
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
					   const vector_t<leveltileset_t>& levelTilesets,
					   const tilesetmap_t<tileset_t>& tilesets,
					   const decltype(tmx_t::layers)& tmxLayers,
					   const decltype(tmx_t::externalTilesets)& tmxTilesets,
					   levelid_t levelID) {

	auto& layers = state[levelID];
	eastl::transform(tmxLayers.begin(), tmxLayers.end(), eastl::back_inserter(layers), [&](const tmxlayer_t& tmxlayer) {

			layer_t layer = {
				.tiles = {},
				.layerIndex = tmxlayer.layerIndex,
				.size = glm::ivec2(tmxlayer.width, tmxlayer.height)
			};

			eastl::transform(tmxlayer.data.tiles.begin(),
							 tmxlayer.data.tiles.end(),
							 eastl::back_inserter(layer.tiles),
							 [&](tmxtile_t tile) {

								 if (tile.gid == 0) {
									 return tileid_t();
								 }

								 int tilesetIndex = -1, i = 0;
								 for (const auto& levelTileset : tmxTilesets) {
									 if (tile.gid >= levelTileset.firstgid) {
										 tilesetIndex = i;
										 break;
									 }
									 ++i;
								 }
								 assert(tilesetIndex != -1);
								 tileid_t tileID({ levelTilesets[tilesetIndex].tileset, tile.gid - tmxTilesets[tilesetIndex].firstgid });
								 return tileid_t(tileID);
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
		auto tilesetIDIter = tilesetIDs.find_as(("tiled/" + tileset.source).c_str());
		assert(tilesetIDIter != tilesetIDs.end());

		tilesetid_t tilesetID = tilesetIDIter->second;
		leveltileset_t levelTileset = {
			.tileset = tilesetID
		};
		layerTilesets.push_back(levelTileset);
	}
}

static void loadPlatformIndex(levelmap_t<int>& state,
							  const decltype(tmx_t::layers)& tmxLayers,
							  levelid_t levelID) {
	int i = 0;
	for (const auto& layer : tmxLayers) {
		if (layer.name == "platform") {
			state[levelID] = i;
			break;
		}
		++i;
	}
}

void loadObjects(levelobjectmap_t<levelobject_t>& state,
				 const decltype(tmx_t::objectgroups)& groups,
				 const decltype(tmx_t::externalTilesets)& tmxTilesets,
				 const vector_t<leveltileset_t>& tilesets,
				 levelid_t levelID,
				 const stringmap_t<animctrlid_t>& controllers,
				 const stringmap_t<colliderid_t>& colliders) {

	assert(tmxTilesets.size() == tilesets.size());

	for (const auto& group : groups) {
		for (const auto& object : group.objects) {

			const tmxexternaltileset_t *pTmxTileset = nullptr;
			const leveltileset_t *pTileset = nullptr;
			for (int i = 0, size = tmxTilesets.size(); i < size; ++i) {
				if (object.gid >= tmxTilesets[i].firstgid) {
					pTmxTileset = &tmxTilesets[i];
					pTileset = &tilesets[i];
				}
			}

			levelobject_t levelObject = {
				.translation = { object.x, object.y - object.height, group.layerIndex },
				.tile = tileid_t({ pTileset->tileset, object.gid - pTmxTileset->firstgid })
			};

			auto animCtrlPropIt = object.properties.find("animctrl");
			if (animCtrlPropIt != object.properties.end()) {
				auto controllerIt = controllers.find_as(animCtrlPropIt->second.c_str());
				assert(controllerIt != controllers.end());
				levelObject.animationController = controllerIt->second;
			}

			auto colliderPropIt = object.properties.find("collider");
			if (colliderPropIt != object.properties.end()) {
				auto colliderIt = colliders.find_as(colliderPropIt->second.c_str());
				assert(colliderIt != colliders.end());
				levelObject.collider = colliderIt->second;
			}

			levelobjectid_t id({ levelID, object.id });
			state.insert({ id, eastl::move(levelObject) });
		}
	}
}

static void loadPlayerObject(levelmap_t<int>& playerObject, const decltype(tmx_t::objectgroups)& groups, levelid_t levelID) {
	for (const auto& group : groups) {
		auto playerIt = eastl::find_if(group.objects.begin(), group.objects.end(), [](const auto& object) {
				return object.name == "player";
			});
		if (playerIt != group.objects.end()) {
			playerObject[levelID] = playerIt->id;
		}
	}
}

void loadLevel(levelstate_t& state, const tmx_t& tmx, const char *pathname, const tilesetstate_t& tilesetState) {
	bool loaded = loadSource(state.source, state.nextLevelID, pathname);
	if (!loaded) {
		return;
	}

	levelid_t levelID = state.source.find_as(pathname)->second;
	loadMap(state.map, tmx, levelID);
	loadTilesets(state.tilesets, tmx.externalTilesets, levelID, tilesetState.source);
	loadLayers(state.layers, state.tilesets.find(levelID)->second, tilesetState.tileset, tmx.layers, tmx.externalTilesets, levelID);
	loadPlatformIndex(state.platformIndex, tmx.layers, levelID);

	loadObjects(state.objects, tmx.objectgroups, tmx.externalTilesets, state.tilesets.find(levelID)->second, levelID, tilesetState.controllerID, tilesetState.colliderID);
	loadPlayerObject(state.playerObject, tmx.objectgroups, levelID);
}

} // namespace te
