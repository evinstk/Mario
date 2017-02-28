#include "game_action.hpp"
#include "world_state.hpp"
#include "util.hpp"
#include "camera.hpp"
#include "game_values.hpp"
#include "entity_state.hpp"
#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include <EASTL/algorithm.h>
#include <EASTL/iterator.h>
#include <SDL.h>

namespace te {

static void stepMode(worldmode_t& modeState,
					 float& elapsedState,
					 float dt) {

	elapsedState += dt;

	if (gWorld.deathTrigger) {
		modeState = worldmode_t::DEATH;
		elapsedState = 0;
	}
}

static void stepView(glm::imat4& state,
					 float dt) {
	static constexpr int HALF_CAMERA = (CAMERA_WIDTH / 2) - 16;
	static constexpr int PUSH_THRESHOLD = HALF_CAMERA - 32;

	glm::vec3 playerTranslation = gEntity.translations.find(gWorld.playerEntity)->second;
	glm::vec3 playerView = state * glm::vec4(playerTranslation, 1.0f);
	float xPlayerVelocity = gEntity.velocities.find(gWorld.playerEntity)->second.x;

	if (playerView.x > HALF_CAMERA) {
		state = glm::translate(state, glm::ivec3(HALF_CAMERA - playerView.x, 0, 0));
	} else if (playerView.x > PUSH_THRESHOLD && xPlayerVelocity > 0) {
		float pushFactor = (playerView.x / HALF_CAMERA) * 0.75f;
		state = glm::translate(state, glm::ivec3(-xPlayerVelocity * dt * pushFactor, 0, 0));
	}
}

static void stepScore(int& score, int& coinCount, int& lives) {
	for (entityid_t blockID : gEntity.hitGround) {
		prize_t prize;
		if (hasPrize(blockID, prize) && prize == prize_t::COIN) {
			score += COIN_SCORE;
			++coinCount;
		}
	}

	if (coinCount >= COIN_COUNT_LIVE_UP) {
		++lives;
		coinCount %= COIN_COUNT_LIVE_UP;
	}
}

static void stepDeathTrigger(bool& state, const gamestate_t& game) {
	state = false;

	float yPos = getTranslation(gWorld.playerEntity).y;
	const auto& map = getMap(game);
	if (yPos > map.size.y * map.tileSize.y && gWorld.mode == worldmode_t::PLAY) {
		state = true;
	}
}

static void stepNewEntityQueue(vector_t<entityrequest_t>& state, const gamestate_t& game) {
	for (entityid_t blockID : gEntity.hitGround) {
		prize_t prize;
		if (hasPrize(blockID, prize) && prize == prize_t::COIN) {
			float yTileSize = getMap(game).tileSize.y;
			entityrequest_t newEntity = {
				.type = entity_t::BLOCK_COIN,
				.translation = getTranslation(blockID) + glm::vec3(0, -yTileSize, 0)
			};
			state.push_back(newEntity);
		}
	}
}

static void stepDestroyQueue(entityset_t& state) {
	for (const auto& lifetimeRow : gEntity.lifetimes) {
		entityid_t entityID = lifetimeRow.first;
		float timeLeft = lifetimeRow.second;
		if (timeLeft <= 0) {
			state.insert(entityID);
		}
	}
}

static void stepSoundQueue(vector_t<soundid_t>& state, const gamestate_t& game) {
	for (entityid_t entityID : gEntity.hitGround) {
		auto soundIt = gEntity.bounceSounds.find(entityID);
		if (soundIt != gEntity.bounceSounds.end() && canBounce(entityID)) {
			state.push_back(soundIt->second);
		}
	}

	if (gWorld.deathTrigger) {
		auto soundIt = game.level.dieMusic.find(gWorld.level);
		if (soundIt != game.level.dieMusic.end()) {
			state.push_back(soundIt->second);
		}
	}
}

void stepWorld(worldstate_t& state, float dt, const gamestate_t& gameState) {
	stepMode(state.mode, state.modeElapsed, dt);
	stepEntity(dt, gameState);
	stepScore(state.score, state.coinCount, state.lives);
	stepView(state.view, dt);
	stepDeathTrigger(state.deathTrigger, gameState);
	stepNewEntityQueue(state.newEntityQueue, gameState);
	stepDestroyQueue(state.destroyQueue);
	stepSoundQueue(state.soundQueue, gameState);
}

} // namespace te
