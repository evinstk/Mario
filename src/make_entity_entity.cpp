#include "game_action.hpp"
#include "entity_state.hpp"
#include "game_state.hpp"
#include "game_values.hpp"
#include <cassert>

namespace te {

static void makeBlockCoin(entitystate_t& state,
						  entityid_t entityID,
						  const glm::vec3& translation,
						  const gamestate_t& game) {

	static constexpr bounceanim_t coinBounceAnim = {
		.duration = COIN_BOUNCE_DURATION,
		.height   = COIN_BOUNCE_HEIGHT,
		.elapsed  = 0
	};

	state.translations[entityID] = translation;
	state.bounceAnimations[entityID] = coinBounceAnim;
	// TODO: replace
	const animid_t& animID = game.tileset.animationID.find_as("coin")->second;
	const animation_t& animation = game.tileset.animation.find(animID)->second;
	state.tilesetSprites[entityID] = tileid_t({ animID.id.first, animation.frames[0].tileid });
}

void makeEntity(entitystate_t& state,
				entityid_t entityID,
				entity_t entity,
				const glm::vec3& translation,
				const gamestate_t& game) {

	switch (entity) {
	case entity_t::BLOCK_COIN:
		makeBlockCoin(state, entityID, translation, game);
		break;
	default:
		assert(false);
	}
}

} // namespace te
