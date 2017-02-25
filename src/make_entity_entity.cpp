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

//static void makeBlockCoin(entitystate_t& state,
//						  entityid_t entityID,
//						  const glm::vec3& translation,
//						  const gamestate_t& game) {
//
//	static constexpr bounceanim_t coinBounceAnim = {
//		.duration = COIN_BOUNCE_DURATION,
//		.height   = COIN_BOUNCE_HEIGHT,
//		.elapsed  = 0
//	};
//
//	state.translations[entityID] = translation;
//	state.bounceAnimations[entityID] = coinBounceAnim;
//	state.lifetimes[entityID] = COIN_BOUNCE_DURATION;
//	// TODO: replace
//	const animid_t& animID = game.tileset.animationID.find_as("coin")->second;
//	const animation_t& animation = game.tileset.animation.find(animID)->second;
//	state.tilesetSprites[entityID] = tileid_t({ animID.id.first, animation.frames[0].tileid });
//}
//
//void makeEntity(entitystate_t& state,
//				entityid_t entityID,
//				entity_t entity,
//				const glm::vec3& translation,
//				const gamestate_t& game) {
//
//	switch (entity) {
//	case entity_t::BLOCK_COIN:
//		makeBlockCoin(state, entityID, translation, game);
//		break;
//	default:
//		assert(false);
//	}
//}

static void makeEntityTranslations(entitymap_t<glm::vec3>& state, const gamestate_t& game) {
	auto idIt = game.world.newEntityIDs.begin();
	for (const auto& request : game.world.newEntityQueue) {
		state[*idIt++] = request.translation;
	}
}

static void makeEntityBounceAnimations(entitymap_t<bounceanim_t>& state, const gamestate_t& game) {
	auto idIt = game.world.newEntityIDs.begin();
	for (const auto& request : game.world.newEntityQueue) {
		if (request.type == entity_t::BLOCK_COIN) {
			state[*idIt] = coinBounceAnim;
		}
		++idIt;
	}
}

static void makeEntityLifetimes(entitymap_t<float>& state, const gamestate_t& game) {
	auto idIt = game.world.newEntityIDs.begin();
	for (const auto& request : game.world.newEntityQueue) {
		if (request.type == entity_t::BLOCK_COIN) {
			state[*idIt] = COIN_BOUNCE_DURATION;
		}
		++idIt;
	}
}

static void makeEntitySprites(entitymap_t<tileid_t>& state, const gamestate_t& game) {
	auto idIt = game.world.newEntityIDs.begin();
	for (const auto& request : game.world.newEntityQueue) {
		if (request.type == entity_t::BLOCK_COIN) {
			const animid_t& animID = game.tileset.animationID.find_as("coin")->second;
			const animation_t& animation = game.tileset.animation.find(animID)->second;
			state[*idIt] = animation.frames[0].tileid;
		}
		++idIt;
	}
}

void makeEntity(entitystate_t& state, const gamestate_t& game) {
	makeEntityTranslations(state.translations, game);
	makeEntityBounceAnimations(state.bounceAnimations, game);
	makeEntityLifetimes(state.lifetimes, game);
	makeEntitySprites(state.tilesetSprites, game);
}

} // namespace te
