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

void worldstate_t::stepMode(worldmode_t& modeState,
							float& elapsedState,
							float dt) const {

	elapsedState += dt;

	if (pGame->world.deathTrigger) {
		modeState = worldmode_t::DEATH;
		elapsedState = 0;
	}
}

void worldstate_t::stepView(glm::imat4& state, float dt) const {
	static constexpr int HALF_CAMERA = (CAMERA_WIDTH / 2) - 16;
	static constexpr int PUSH_THRESHOLD = HALF_CAMERA - 32;

	glm::vec3 playerTranslation = pGame->world.entity.translations.find(pGame->world.playerEntity)->second;
	glm::vec3 playerView = state * glm::vec4(playerTranslation, 1.0f);
	float xPlayerVelocity = pGame->world.entity.velocities.find(pGame->world.playerEntity)->second.x;

	if (playerView.x > HALF_CAMERA) {
		state = glm::translate(state, glm::ivec3(HALF_CAMERA - playerView.x, 0, 0));
	} else if (playerView.x > PUSH_THRESHOLD && xPlayerVelocity > 0) {
		float pushFactor = (playerView.x / HALF_CAMERA) * 0.75f;
		state = glm::translate(state, glm::ivec3(-xPlayerVelocity * dt * pushFactor, 0, 0));
	}
}

void worldstate_t::stepScore(int& score, int& coinCount, int& lives) const {
	for (entityid_t blockID : pGame->world.entity.hitGround) {
		prize_t prize;
		if (hasPrize(blockID, prize, *pGame) && prize == prize_t::COIN) {
			score += COIN_SCORE;
			++coinCount;
		}
	}

	if (coinCount >= COIN_COUNT_LIVE_UP) {
		++lives;
		coinCount %= COIN_COUNT_LIVE_UP;
	}
}

void worldstate_t::stepDeathTrigger(bool& state) const {
	state = false;

	float yPos = getTranslation(pGame->world.playerEntity, *pGame).y;
	const auto& map = getMap(*pGame);
	if (yPos > map.size.y * map.tileSize.y && pGame->world.mode == worldmode_t::PLAY) {
		state = true;
	}
}

void worldstate_t::stepNewEntityQueue(vector_t<entityrequest_t>& state) const {
	for (entityid_t blockID : pGame->world.entity.hitGround) {
		prize_t prize;
		if (hasPrize(blockID, prize, *pGame) && prize == prize_t::COIN) {
			float yTileSize = getMap(*pGame).tileSize.y;
			entityrequest_t newEntity = {
				.type = entity_t::BLOCK_COIN,
				.translation = getTranslation(blockID, *pGame) + glm::vec3(0, -yTileSize, 0)
			};
			state.push_back(newEntity);
		}
	}
}

void worldstate_t::stepDestroyQueue(entityset_t& state) const {
	for (const auto& lifetimeRow : pGame->world.entity.lifetimes) {
		entityid_t entityID = lifetimeRow.first;
		float timeLeft = lifetimeRow.second;
		if (timeLeft <= 0) {
			state.insert(entityID);
		}
	}
}

void worldstate_t::step(float dt) {
	stepMode(mode, modeElapsed, dt);
	stepEntity(entity, dt, *pGame);
	stepScore(score, coinCount, lives);
	stepView(view, dt);
	stepDeathTrigger(deathTrigger);
	stepNewEntityQueue(newEntityQueue);
	stepDestroyQueue(destroyQueue);
}

} // namespace te
