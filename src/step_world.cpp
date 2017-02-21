#include "game_action.hpp"
#include "world_state.hpp"
#include "util.hpp"
#include "camera.hpp"
#include "game_values.hpp"
#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include <EASTL/algorithm.h>
#include <EASTL/iterator.h>
#include <SDL.h>

namespace te {

static void stepView(glm::imat4& state,
					 float dt,
					 const gamestate_t& game) {
	static constexpr int HALF_CAMERA = (CAMERA_WIDTH / 2) - 16;
	static constexpr int PUSH_THRESHOLD = HALF_CAMERA - 32;

	glm::vec3 playerTranslation = game.world.entity.translations.find(game.world.playerEntity)->second;
	glm::vec3 playerView = state * glm::vec4(playerTranslation, 1.0f);
	float xPlayerVelocity = game.world.entity.velocities.find(game.world.playerEntity)->second.x;

	if (playerView.x > HALF_CAMERA) {
		state = glm::translate(state, glm::ivec3(HALF_CAMERA - playerView.x, 0, 0));
	} else if (playerView.x > PUSH_THRESHOLD && xPlayerVelocity > 0) {
		float pushFactor = (playerView.x / HALF_CAMERA) * 0.75f;
		state = glm::translate(state, glm::ivec3(-xPlayerVelocity * dt * pushFactor, 0, 0));
	}
}

static void stepScore(int& score, int& coinCount, int& lives, const gamestate_t& game) {
	for (entityid_t blockID : game.world.entity.hitGround) {
		prize_t prize;
		if (hasPrize(blockID, prize, game) && prize == prize_t::COIN) {
			score += COIN_SCORE;
			++coinCount;
		}
	}

	if (coinCount >= COIN_COUNT_LIVE_UP) {
		++lives;
		coinCount %= COIN_COUNT_LIVE_UP;
	}
}

static void stepNewEntityQueue(vector_t<entityrequest_t>& state, const gamestate_t& game) {
	for (entityid_t blockID : game.world.entity.hitGround) {
		prize_t prize;
		if (hasPrize(blockID, prize, game) && prize == prize_t::COIN) {
			float yTileSize = getMap(game).tileSize.y;
			entityrequest_t newEntity = {
				.type = entity_t::BLOCK_COIN,
				.translation = getTranslation(blockID, game) + glm::vec3(0, -yTileSize, 0)
			};
			state.push_back(newEntity);
		}
	}
}

static void stepDestroyQueue(entityset_t& state, const gamestate_t& game) {
	for (const auto& lifetimeRow : game.world.entity.lifetimes) {
		entityid_t entityID = lifetimeRow.first;
		float timeLeft = lifetimeRow.second;
		if (timeLeft <= 0) {
			state.insert(entityID);
		}
	}
}

static void stepSoundQueue(vector_t<soundid_t>& state, const gamestate_t& game) {
	state.clear();
	for (entityid_t entityID : game.world.entity.hitGround) {
		auto soundIt = game.world.entity.bounceSounds.find(entityID);
		if (soundIt != game.world.entity.bounceSounds.end() && canBounce(entityID, game)) {
			state.push_back(soundIt->second);
		}
	}
}

void stepWorld(worldstate_t& state, float dt, const gamestate_t& gameState) {
	stepEntity(state.entity, dt, gameState);
	stepScore(state.score, state.coinCount, state.lives, gameState);
	stepView(state.view, dt, gameState);
	stepNewEntityQueue(state.newEntityQueue, gameState);
	stepDestroyQueue(state.destroyQueue, gameState);
	stepSoundQueue(state.soundQueue, gameState);
}

} // namespace te
