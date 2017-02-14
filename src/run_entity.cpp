#include "reducer_world.hpp"
#include "entity_state.hpp"
#include "level_state.hpp"
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

template <typename Iter>
void runTranslations(entitymap_t<glm::vec3>& state, Iter first, Iter last) {
	for (auto it = first; it != last; ++it) {
		entity_t id(it->first.id.second);
		state.insert({ id, it->second.translation });
	}

	//MapRange range(levelState.objects.begin(), levelState.objects.end(), levelID);
	//for (const auto& row : range) {
	//	entity_t id(row.first.id.second);
	//	state.insert({ id, row.second.translation });
	//}
}

template <typename Iter>
void runTilesetSprites(entitymap_t<tileid_t>& state, Iter first, Iter last) {
	for (auto it = first; it != last; ++it) {
		tileid_t tile = it->second.tile;
		if (tile.id.first.id > 0) {
			entity_t id(it->first.id.second);
			state.insert({ id, tile });
		}
	}
}

template <typename Iter>
void runAnimators(entitymap_t<animator_t>& state, Iter first, Iter last) {
	for (auto it = first; it != last; ++it) {
		animctrlid_t ctrl = it->second.animationController;
		if (ctrl.id > 0) {
			entity_t id(it->first.id.second);
			animator_t animator = {
				.controller = ctrl,
				.animation = {},
				.elapsed = 0
			};
			state.insert({ id, animator });
		}
	}
}

void runEntity(entitystate_t& state, levelid_t levelID, const levelstate_t& levelState) {
	auto lowerBound = levelState.objects.lower_bound(levelobjectid_t({ levelID, 0 }));
	auto upperBound = levelState.objects.upper_bound(levelobjectid_t({ levelID.next(), 0 }));
	runTranslations(state.translations, lowerBound, upperBound);
	runTilesetSprites(state.tilesetSprites, lowerBound, upperBound);
	runAnimators(state.animators, lowerBound, upperBound);
}

} // namespace te
