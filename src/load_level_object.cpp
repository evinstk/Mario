#include "game_action.hpp"
#include "level_object_state.hpp"
#include "game_state.hpp"
#include "util.hpp"
#include <tegl/readtmx.hpp>

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

template <typename ID>
void load(levelobjectmap_t<ID>& state,
		  const tmx_t& tmx,
		  levelid_t levelID,
		  const char *prop,
		  const stringmap_t<ID>& lookup) {

	for (const auto& group : tmx.objectgroups) {
		for (const auto& object : group.objects) {
			auto propIt = object.properties.find(prop);
			if (propIt != object.properties.end()) {
				levelobjectid_t objectID({ levelID, object.id });
				ID animID = lookup.find_as(propIt->second.c_str())->second;
				state.insert({ objectID, animID });
			}
		}
	}
}

static void loadColliders(levelobjectmap_t<colliderid_t>& state,
						  const tmx_t& tmx,
						  levelid_t levelID,
						  const gamestate_t& game) {
	load(state, tmx, levelID, "collider", game.tileset.colliderID);
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

static void loadCanBounce(levelobjectset_t& state,
						  const tmx_t& tmx,
						  levelid_t levelID) {
	for (const auto& group : tmx.objectgroups) {
		for (const auto& object : group.objects) {
			auto canBouncePropIt = object.bProperties.find("can-bounce");
			if (canBouncePropIt != object.bProperties.end() && canBouncePropIt->second) {
				levelobjectid_t objectID({ levelID, object.id });
				state.insert(objectID);
			}
		}
	}
}

static void loadBounceNum(levelobjectmap_t<int>& state,
						  const tmx_t& tmx,
						  levelid_t levelID) {
	for (const auto& group : tmx.objectgroups) {
		for (const auto& object : group.objects) {
			auto bounceNumPropIt = object.iProperties.find("bounce-num");
			if (bounceNumPropIt != object.iProperties.end()) {
				levelobjectid_t objectID({ levelID, object.id });
				state.insert({ objectID, bounceNumPropIt->second });
			}
		}
	}
}

static void loadPrizeNum(levelobjectmap_t<int>& state,
						 const tmx_t& tmx,
						 levelid_t levelID) {
	for (const auto& group : tmx.objectgroups) {
		for (const auto& object : group.objects) {
			auto bounceNumPropIt = object.iProperties.find("prize-num");
			if (bounceNumPropIt != object.iProperties.end()) {
				levelobjectid_t objectID({ levelID, object.id });
				state.insert({ objectID, bounceNumPropIt->second });
			}
		}
	}
}

static inline void loadEmptyTiles(levelobjectmap_t<tileid_t>& state,
								  const tmx_t& tmx,
								  levelid_t levelID,
								  const gamestate_t& game) {
	load(state, tmx, levelID, "empty-tile", game.tileset.tileID);
}

void levelobjectstate_t::load(const tmx_t& tmx, levelid_t levelID) {
	loadTranslations(translations, tmx, levelID);
	loadTiles(tiles, tmx, levelID, *pGame);

	te::load(animations, tmx, levelID, "animation", pGame->tileset.animationID);
	te::load(animationsLeft, tmx, levelID, "anim-left", pGame->tileset.animationID);
	te::load(animationsRight, tmx, levelID, "anim-right", pGame->tileset.animationID);

	loadColliders(colliders, tmx, levelID, *pGame);
	loadGravities(gravities, tmx, levelID);
	loadGrounds(grounds, tmx, levelID);

	loadPrizes(prizes, tmx, levelID);
	loadPrizeNum(prizeNum, tmx, levelID);
	loadEmptyTiles(emptyTiles, tmx, levelID, *pGame);

	loadBounceSounds(bounceSounds, tmx, levelID, *pGame);
	loadCanBounce(canBounce, tmx, levelID);
	loadBounceNum(bounceNum, tmx, levelID);
}

} // namespace te
