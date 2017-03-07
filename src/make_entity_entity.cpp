#include "game_action.hpp"
#include "entity_state.hpp"
#include "game_state.hpp"
#include "game_values.hpp"
#include <cassert>

namespace te {

static constexpr bounceanim_t coinBounceAnim = {
	.duration = COIN_BOUNCE_DURATION,
	.height   = COIN_BOUNCE_HEIGHT,
	.elapsed  = 0
};

void entitystate_t::makeEntityTranslations(entitymap_t<glm::vec3>& state) const {
	auto idIt = pGame->world.newEntityIDs.begin();
	for (const auto& request : pGame->world.newEntityQueue) {
		state[*idIt++] = request.translation;
	}
}

void entitystate_t::makeEntityBounceAnimations(entitymap_t<bounceanim_t>& state) const {
	auto idIt = pGame->world.newEntityIDs.begin();
	for (const auto& request : pGame->world.newEntityQueue) {
		if (request.type == entity_t::BLOCK_COIN) {
			state[*idIt] = coinBounceAnim;
		}
		++idIt;
	}
}

void entitystate_t::makeEntityLifetimes(entitymap_t<float>& state) const {
	auto idIt = pGame->world.newEntityIDs.begin();
	for (const auto& request : pGame->world.newEntityQueue) {
		if (request.type == entity_t::BLOCK_COIN) {
			state[*idIt] = COIN_BOUNCE_DURATION;
		}
		++idIt;
	}
}

void entitystate_t::makeEntityAnimators(entitymap_t<spriteanimator_t>& state) const {
	auto idIt = pGame->world.newEntityIDs.begin();
	for (const auto& request : pGame->world.newEntityQueue) {
		if (request.type == entity_t::BLOCK_COIN) {
			animid_t animID = pGame->tileset.animationID.find_as("coin")->second;
			spriteanimator_t& animator = state[*idIt];

			animator.animation = animID;
			animator.elapsed = 0;
		}
		++idIt;
	}
}

void entitystate_t::makeEntity() {
	makeEntityTranslations(translations);
	makeEntityBounceAnimations(bounceAnimations);
	makeEntityLifetimes(lifetimes);
	makeEntityAnimators(spriteAnimators);
}

} // namespace te
