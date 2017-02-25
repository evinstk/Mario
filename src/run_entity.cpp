#include "game_action.hpp"
#include "entity_state.hpp"
#include "level_state.hpp"
#include "util.hpp"
#include <EASTL/algorithm.h>

namespace te {

static void runColliders(entitymap_t<colliderid_t>& state,
						 levelid_t levelID,
						 const levelobjectmap_t<colliderid_t>& colliders) {
	auto range = parentRange(colliders, levelID);
	for (auto it = range.first; it != range.second; ++it) {
		state.insert({ entityid_t(it->first.id.second), it->second });
	}
}

static void runBounceSounds(entitymap_t<soundid_t>& state,
							levelid_t levelID,
							const levelobjectmap_t<soundid_t>& colliders) {
	auto range = parentRange(colliders, levelID);
	for (auto it = range.first; it != range.second; ++it) {
		state.insert({ entityid_t(it->first.id.second), it->second });
	}
}

static void runIsGround(entityset_t& state,
						levelid_t levelID,
						const levelobjectset_t& grounds) {
	auto range = parentRange(grounds, levelID);
	for (auto it = range.first; it != range.second; ++it) {
		state.insert(entityid_t(it->id.second));
	}
}

static void runUnderGravity(entityset_t& state,
							levelid_t levelID,
							const levelobjectset_t& gravities) {
	auto range = parentRange(gravities, levelID);
	for (auto it = range.first; it != range.second; ++it) {
		state.insert(entityid_t(it->id.second));
	}
}

static void runVelocities(entitymap_t<glm::vec3>& state,
						  levelid_t levelID,
						  const levelobjectmap_t<colliderid_t>& colliders) {
	auto range = parentRange(colliders, levelID);
	for (auto it = range.first; it != range.second; ++it) {
		state.insert({ entityid_t(it->first.id.second), { 0, 0, 0 } });
	}
}

static void runTranslations(entitymap_t<glm::vec3>& state,
							levelid_t levelID,
							const levelobjectmap_t<glm::vec3>& translations) {
	auto range = parentRange(translations, levelID);
	for (auto it = range.first; it != range.second; ++it) {
		state.insert({ entityid_t(it->first.id.second), it->second });
	}
}

static void runTilesetSprites(entitymap_t<tileid_t>& state,
							  levelid_t levelID,
							  const levelobjectmap_t<tileid_t>& tilesets) {
	auto range = parentRange(tilesets, levelID);
	for (auto it = range.first; it != range.second; ++it) {
		state.insert({ entityid_t(it->first.id.second), it->second });
	}
}

static void runPrizes(entitymap_t<prize_t>& state,
					  levelid_t levelID,
					  const levelobjectmap_t<prize_t>& prizes) {

	auto range = parentRange(prizes, levelID);
	for (auto it = range.first; it != range.second; ++it) {
		state.insert({ entityid_t(it->first.id.second), it->second });
	}
}

void transferProperty(entityset_t& state,
					  levelid_t levelID,
					  const levelobjectset_t& props) {
	auto range = parentRange(props, levelID);
	for (auto it = range.first; it != range.second; ++it) {
		state.insert({ entityid_t(it->id.second) });
	}
}

template <typename T>
void transferProperty(entitymap_t<T>& state,
					  levelid_t levelID,
					  const levelobjectmap_t<T>& props) {
	auto range = parentRange(props, levelID);
	for (auto it = range.first; it != range.second; ++it) {
		state.insert({ entityid_t(it->first.id.second), it->second });
	}
}

void runEntity(entitystate_t& state, levelid_t levelID, const levelobjectstate_t& objects) {
	runColliders(state.colliders, levelID, objects.colliders);
	runBounceSounds(state.bounceSounds, levelID, objects.bounceSounds);
	runUnderGravity(state.underGravity, levelID, objects.gravities);
	runIsGround(state.isGround, levelID, objects.grounds);
	runVelocities(state.velocities, levelID, objects.colliders);
	runTranslations(state.translations, levelID, objects.translations);
	runTilesetSprites(state.tilesetSprites, levelID, objects.tiles);
	transferProperty(state.animationsLeft, levelID, objects.animationsLeft);
	transferProperty(state.animationsRight, levelID, objects.animationsRight);
	runPrizes(state.prizes, levelID, objects.prizes);
	transferProperty(state.prizeNum, levelID, objects.prizeNum);
	transferProperty(state.canBounce, levelID, objects.canBounce);
	transferProperty(state.bounceNum, levelID, objects.bounceNum);
}

} // namespace te
