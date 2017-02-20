#include "game_action.hpp"
#include "level_object_state.hpp"
#include "game_state.hpp"
#include "util.hpp"

namespace te {

static void loadTranslations(levelobjectmap_t<glm::vec3>& state,
							 const tmx_t& tmx,
							 levelid_t levelID) {

	for (const auto& group : tmx.objectgroups) {
		for (const auto& object : group.objects) {
			levelobjectid_t objectID({ levelID, object.id });
			glm::vec3 translation = { object.x, object.y - object.height, group.layerIndex };
			state.insert({ objectID, translation });
		}
	}
}

static void loadTiles(levelobjectmap_t<tileid_t>& state,
					  const tmx_t& tmx,
					  levelid_t levelID,
					  const gamestate_t& game) {

	const auto& tmxTilesets = tmx.externalTilesets;
	const auto& tilesets = game.level.tilesets.find(levelID)->second;
	assert(tmxTilesets.size() == tilesets.size());

	for (const auto& group : tmx.objectgroups) {
		for (const auto& object : group.objects) {
			if (object.gid > 0) {
				const tmxexternaltileset_t *pTmxTileset = nullptr;
				const leveltileset_t *pTileset = nullptr;
				for (int i = 0, size = tmxTilesets.size(); i < size; ++i) {
					if (object.gid >= tmxTilesets[i].firstgid) {
						pTmxTileset = &tmxTilesets[i];
						pTileset = &tilesets[i];
					}
				}
				levelobjectid_t objectID({ levelID, object.id });
				tileid_t tileID({ pTileset->tileset, object.gid - pTmxTileset->firstgid });
				state.insert({ objectID, tileID });
			}
		}
	}
}

static void loadAnimationControllers(levelobjectmap_t<animctrlid_t>& state,
									 const tmx_t& tmx,
									 levelid_t levelID,
									 const gamestate_t& game) {
	for (const auto& group : tmx.objectgroups) {
		for (const auto& object : group.objects) {
			auto animCtrlPropIt = object.properties.find("animctrl");
			if (animCtrlPropIt != object.properties.end()) {
				levelobjectid_t objectID({ levelID, object.id });
				animctrlid_t ctrlID = game.tileset.controllerID.find_as(animCtrlPropIt->second.c_str())->second;
				state.insert({ objectID, ctrlID });
			}
		}
	}
}

static void loadColliders(levelobjectmap_t<colliderid_t>& state,
						  const tmx_t& tmx,
						  levelid_t levelID,
						  const gamestate_t& game) {
	for (const auto& group : tmx.objectgroups) {
		for (const auto& object : group.objects) {
			auto colliderPropIt = object.properties.find("collider");
			if (colliderPropIt != object.properties.end()) {
				levelobjectid_t objectID({ levelID, object.id });
				colliderid_t colliderID = game.tileset.colliderID.find_as(colliderPropIt->second.c_str())->second;
				state.insert({ objectID, colliderID });
			}
		}
	}
}

static void loadGravities(levelobjectset_t& state,
						  const tmx_t& tmx,
						  levelid_t levelID) {
	for (const auto& group : tmx.objectgroups) {
		for (const auto& object : group.objects) {
			auto gravityPropIt = object.bProperties.find("gravity");
			if (gravityPropIt != object.bProperties.end() && gravityPropIt->second) {
				levelobjectid_t objectID({ levelID, object.id });
				state.insert(objectID);
			}
		}
	}
}

static void loadGrounds(levelobjectset_t& state,
						const tmx_t& tmx,
						levelid_t levelID) {
	for (const auto& group : tmx.objectgroups) {
		for (const auto& object : group.objects) {
			auto groundPropIt = object.bProperties.find("ground");
			if (groundPropIt != object.bProperties.end() && groundPropIt->second) {
				levelobjectid_t objectID({ levelID, object.id });
				state.insert(objectID);
			}
		}
	}
}

static void loadPrizes(levelobjectmap_t<prize_t>& state,
					   const tmx_t& tmx,
					   levelid_t levelID) {
	for (const auto& group : tmx.objectgroups) {
		for (const auto& object : group.objects) {
			auto prizePropIt = object.properties.find("prize");
			if (prizePropIt != object.properties.end()) {
				levelobjectid_t objectID({ levelID, object.id });
				const std::string& prizeStr = prizePropIt->second;
				if (prizeStr == "coin") {
					state.insert({ objectID, prize_t::COIN });
				} else {
					assert(false);
				}
			}
		}
	}
}

static void loadBounceSounds(levelobjectmap_t<soundid_t>& state,
							 const tmx_t& tmx,
							 levelid_t levelID,
							 const gamestate_t& game) {
	for (const auto& group : tmx.objectgroups) {
		for (const auto& object : group.objects) {
			auto soundPropIt = object.properties.find("bounce-sound");
			if (soundPropIt != object.properties.end()) {
				levelobjectid_t objectID({ levelID, object.id });
				const std::string& soundPathname = "tiled/" + soundPropIt->second;
				soundid_t soundID = getSoundID(soundPathname.c_str(), game);
				state.insert({ objectID, soundID });
			}
		}
	}
}

void loadLevelObjects(levelobjectstate_t& state, const tmx_t& tmx, levelid_t levelID, const gamestate_t& game) {
	loadTranslations(state.translations, tmx, levelID);
	loadTiles(state.tiles, tmx, levelID, game);
	loadAnimationControllers(state.animationControllers, tmx, levelID, game);
	loadColliders(state.colliders, tmx, levelID, game);
	loadGravities(state.gravities, tmx, levelID);
	loadGrounds(state.grounds, tmx, levelID);
	loadPrizes(state.prizes, tmx, levelID);
	loadBounceSounds(state.bounceSounds, tmx, levelID, game);
}

} // namespace te
