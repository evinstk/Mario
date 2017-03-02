#include "game_action.hpp"
#include "entity_state.hpp"
#include "world_state.hpp"
#include "game_values.hpp"
#include "tileset_state.hpp"
#include <cassert>

namespace te {

static constexpr bounceanim_t coinBounceAnim = {
	.duration = COIN_BOUNCE_DURATION,
	.height   = COIN_BOUNCE_HEIGHT,
	.elapsed  = 0
};

static void makeEntityTranslations(entitymap_t<glm::vec3>& state) {
	auto idIt = gWorld.newEntityIDs.begin();
	for (const auto& request : gWorld.newEntityQueue) {
		state[*idIt++] = request.translation;
	}
}

static void makeEntityBounceAnimations(entitymap_t<bounceanim_t>& state) {
	auto idIt = gWorld.newEntityIDs.begin();
	for (const auto& request : gWorld.newEntityQueue) {
		if (request.type == entity_t::BLOCK_COIN) {
			state[*idIt] = coinBounceAnim;
		}
		++idIt;
	}
}

static void makeEntityLifetimes(entitymap_t<float>& state) {
	auto idIt = gWorld.newEntityIDs.begin();
	for (const auto& request : gWorld.newEntityQueue) {
		if (request.type == entity_t::BLOCK_COIN) {
			state[*idIt] = COIN_BOUNCE_DURATION;
		}
		++idIt;
	}
}

static void makeEntityAnimators(entitymap_t<spriteanimator_t>& state) {
	auto idIt = gWorld.newEntityIDs.begin();
	for (const auto& request : gWorld.newEntityQueue) {
		if (request.type == entity_t::BLOCK_COIN) {
			animid_t animID = gTileset.animationID.find_as("coin")->second;
			spriteanimator_t& animator = state[*idIt];

			animator.animation = animID;
			animator.elapsed = 0;
		}
		++idIt;
	}
}

void makeEntity(entitystate_t& state) {
	makeEntityTranslations(state.translations);
	makeEntityBounceAnimations(state.bounceAnimations);
	makeEntityLifetimes(state.lifetimes);
	makeEntityAnimators(state.spriteAnimators);
}

} // namespace te
