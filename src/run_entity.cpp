#include "game_action.hpp"
#include "entity_state.hpp"
#include "level_state.hpp"
#include "util.hpp"
#include <EASTL/algorithm.h>

namespace te {

namespace {
struct ObjectComp {
	template <typename T>
	bool operator()(T&& row, levelid_t levelID) const noexcept {
		return row.first.id.first < levelID;
	}
	template <typename T>
	bool operator()(levelid_t levelID, T&& row) const noexcept {
		return levelID < row.first.id.first;
	}
};

//template <typename Iter, typename FirstK>
//struct MapRange {
//	eastl::pair<Iter, Iter> iterPair;
//	MapRange(Iter first, Iter last, FirstK k) {
//		iterPair = eastl::equal_range(first, last, k, ObjectComp());
//	}
//	Iter begin() const noexcept {
//		return iterPair.first;
//	}
//	Iter end() const noexcept {
//		return iterPair.second;
//	}
//};
} // anon namespace

static void runColliders(entitymap_t<colliderid_t>& state,
						 levelid_t levelID,
						 const levelobjectmap_t<colliderid_t>& colliders) {
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

static void runAnimators(entitymap_t<animator_t>& state,
						 levelid_t levelID,
						 const levelobjectmap_t<animctrlid_t>& controllers) {
	auto range = parentRange(controllers, levelID);
	for (auto it = range.first; it != range.second; ++it) {
		animator_t animator = {
			.controller = it->second,
			.animation = {},
			.elapsed = 0
		};
		state.insert({ entityid_t(it->first.id.second), animator });
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

void runEntity(entitystate_t& state, levelid_t levelID, const levelobjectstate_t& objects) {
	runColliders(state.colliders, levelID, objects.colliders);
	runUnderGravity(state.underGravity, levelID, objects.gravities);
	runIsGround(state.isGround, levelID, objects.grounds);
	runVelocities(state.velocities, levelID, objects.colliders);
	runTranslations(state.translations, levelID, objects.translations);
	runTilesetSprites(state.tilesetSprites, levelID, objects.tiles);
	runAnimators(state.animators, levelID, objects.animationControllers);
	runPrizes(state.prizes, levelID, objects.prizes);
}

} // namespace te
