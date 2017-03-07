#include "game_action.hpp"
#include "entity_state.hpp"
#include "game_state.hpp"
#include "tileset_state.hpp"
#include "level_state.hpp"
#include "util.hpp"
#include "game_values.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace te {

static int senseOnWall(int y,
					   int xStart,
					   int xEnd,
					   int xSize,
					   int ySize,
					   const layer_t& platformLayer,
					   const eastl::vector_set<tileid_t>& solids) {

	int yTile = y / ySize;
	bool pos = xEnd > xStart;

	for (int xTile = xStart / xSize, end = xEnd / xSize; pos ? xTile <= end : xTile >= end; pos ? ++xTile : --xTile) {

		int tileIndex = yTile * platformLayer.size.x + xTile;
		if (tileIndex >= platformLayer.tiles.size()) {
			continue;
		}

		tileid_t tile = platformLayer.tiles[tileIndex];
		if (solids.find(tile) != solids.end()) {
			return xTile * xSize;
		}
	}

	return -1;
}

void entitystate_t::stepWallOffsets(entitymap_t<float>& state, float dt) const {
	state.clear();

	const layer_t& platformLayer = getPlatformLayer(*pGame);
	glm::vec2 tileSize = getMap(*pGame).tileSize;

	for (const auto& colliderRow : pGame->world.entity.colliders) {
		entityid_t entityID = colliderRow.first;
		colliderid_t colliderID = colliderRow.second;
		const aabb_t& collider = pGame->tileset.collider.find(colliderID)->second;
		glm::vec3 velocity = pGame->world.entity.velocities.find(entityID)->second;
		glm::vec3 translation = pGame->world.entity.translations.find(entityID)->second + velocity * dt;

		glm::vec2 halfSize = collider.size / glm::vec2(2, 2);
		glm::vec2 start = glm::vec2(translation) + collider.pos + halfSize;
		glm::vec2 end1  = start - glm::vec2(halfSize.x, 0);
		glm::vec2 end2  = start + glm::vec2(halfSize.x, 0);

		int xWallPos1 = senseOnWall(start.y, start.x, end1.x, tileSize.x, tileSize.y, platformLayer, pGame->tileset.solid);
		int xWallPos2 = senseOnWall(start.y, start.x, end2.x, tileSize.x, tileSize.y, platformLayer, pGame->tileset.solid);

		float offset = 0;
		if (xWallPos1 != -1) {
			offset = xWallPos1 + tileSize.x - translation.x - collider.pos.x;
		} else if (xWallPos2 != -1) {
			offset = xWallPos2 - translation.x - collider.pos.x - collider.size.x;
		} else {
			aabb_t entityCollider = collider;
			entityCollider.pos += glm::vec2(translation);
			entityCollider.pos.y += entityCollider.size.y / 2;
			entityCollider.size.y = 1;
			for (entityid_t groundID : pGame->world.entity.isGround) {
				aabb_t groundCollider = getCollider(groundID, *pGame);
				groundCollider.pos += glm::vec2(getTranslation(groundID, *pGame));
				if (groundID != entityID && isColliding(entityCollider, groundCollider)) {
					//offset = groundCollider.pos.x - translation.x - collider.pos.x - collider.size.x;
					if (translation.x > groundCollider.pos.x) {
						offset = groundCollider.pos.x + groundCollider.size.x - translation.x - collider.pos.x;
						//offset = -offset;
						//break;
					} else {
						offset = groundCollider.pos.x - translation.x - collider.pos.x - collider.size.x;
					}
				}
			}
		}
		state.insert({ entityID, offset });
	}
}

static int senseGround(int x,
					   int yStart,
					   int yEnd,
					   int xSize,
					   int ySize,
					   const layer_t& platformLayer,
					   const eastl::vector_set<tileid_t>& solids) {

	int xTile = x / xSize;
	bool pos = yEnd > yStart;

	for (int yTile = yStart / ySize, end = yEnd / ySize; pos ? yTile <= end : yTile >= end; pos ? ++yTile : --yTile) {

		int tileIndex = yTile * platformLayer.size.x + xTile;
		if (tileIndex >= platformLayer.tiles.size()) {
			continue;
		}

		tileid_t tile = platformLayer.tiles[tileIndex];
		if (solids.find(tile) != solids.end()) {
			return yTile * ySize;
		}
	}

	return -1;
}

void entitystate_t::stepColliders(entitymap_t<float>& groundOffsets, float dt) const {
	groundOffsets.clear();

	glm::ivec2 tileSize = getMap(*pGame).tileSize;
	const layer_t& platformLayer = getPlatformLayer(*pGame);

	for (entityid_t entityID : pGame->world.entity.underGravity) {

		glm::vec3 velocity = pGame->world.entity.velocities.find(entityID)->second;
		if (velocity.y < 0) {
			continue;
		}

		colliderid_t colliderID = pGame->world.entity.colliders.find(entityID)->second;
		const aabb_t& collider = pGame->tileset.collider.find(colliderID)->second;
		glm::vec3 translation = getTranslation(entityID, *pGame) + velocity * dt;
		translation.x += pGame->world.entity.wallOffsets.find(entityID)->second;

		int halfColliderSizeY = collider.size.y / 2;
		int yStart = translation.y + collider.pos.y + halfColliderSizeY;
		int yEnd   = yStart + halfColliderSizeY;
		int x1     = translation.x + collider.pos.x + 1;
		int x2     = translation.x + collider.pos.x + collider.size.x - 1;

		int solidTile1 = senseGround(x1, yStart, yEnd, tileSize.x, tileSize.y, platformLayer, pGame->tileset.solid);
		int solidTile2 = senseGround(x2, yStart, yEnd, tileSize.x, tileSize.y, platformLayer, pGame->tileset.solid);

		if (solidTile1 >= 0) {
			groundOffsets.insert({ entityID, solidTile1 - translation.y - collider.pos.y - collider.size.y });
		} else if (solidTile2 >= 0) {
			groundOffsets.insert({ entityID, solidTile2 - translation.y - collider.pos.y - collider.size.y });
		} else {

			bool onGround = false;
			float offset = 0;
			aabb_t entityCollider = collider;
			entityCollider.pos += glm::vec2(translation);
			entityCollider.pos.y += collider.size.y / 2;
			entityCollider.size.y = (collider.size.y / 2) + 1;
			for (entityid_t groundID : pGame->world.entity.isGround) {
				aabb_t groundCollider = getCollider(groundID, *pGame);
				groundCollider.pos += glm::vec2(getTranslation(groundID, *pGame));
				if (isColliding(entityCollider, groundCollider)) {
					offset = groundCollider.pos.y - translation.y - collider.pos.y - collider.size.y;
					onGround = true;
					break;
				}
			}
			if (onGround) {
				groundOffsets.insert({ entityID, offset });
			}
		}
	}
}

void entitystate_t::stepCeilingOffsets(entitymap_t<float>& ceilingOffsets,
									   entityset_t& hitGround,
									   float dt) const {
	ceilingOffsets.clear();
	hitGround.clear();

	for (entityid_t entityID : pGame->world.entity.underGravity) {
		glm::vec2 velocity = getVelocity(entityID, *pGame);
		if (velocity.y >= 0) {
			continue;
		}

		aabb_t collider = getCollider(entityID, *pGame);
		float halfColliderSizeY = collider.size.y * 0.5f;
		glm::vec2 translation = glm::vec2(getTranslation(entityID, *pGame)) + velocity * dt;
		translation.x += getWallOffset(entityID, *pGame);
		glm::vec2 tileSize = getMap(*pGame).tileSize;
		const layer_t& platformLayer = getPlatformLayer(*pGame);

		glm::vec2 start1 = translation + collider.pos + glm::vec2(2.0f, halfColliderSizeY);
		glm::vec2 end1   = translation + collider.pos + glm::vec2(2.0f, 0.0f);
		glm::vec2 start2 = translation + collider.pos + glm::vec2(collider.size.x - 3.0f, halfColliderSizeY);
		glm::vec2 end2   = translation + collider.pos + glm::vec2(collider.size.x - 3.0f, 0.0f);

		int solidTile1 = senseGround(start1.x, start1.y, end1.y, tileSize.x, tileSize.y, platformLayer, pGame->tileset.solid);
		int solidTile2 = senseGround(start2.x, start2.y, end2.y, tileSize.x, tileSize.y, platformLayer, pGame->tileset.solid);

		if (solidTile1 >= 0) {
			ceilingOffsets.insert({ entityID, solidTile1 + tileSize.y - translation.y - collider.pos.y });
		} else if (solidTile2 >= 0) {
			ceilingOffsets.insert({ entityID, solidTile2 + tileSize.y - translation.y - collider.pos.y });
		} else {
			aabb_t entityCollider = collider;
			entityCollider.pos += translation;
			entityCollider.size.y /= 2;
			for (entityid_t groundID : pGame->world.entity.isGround) {
				glm::vec2 groundTranslation = getTranslation(groundID, *pGame);
				aabb_t groundCollider = getCollider(groundID, *pGame);
				groundCollider.pos += groundTranslation;
				if (isColliding(entityCollider, groundCollider)) {
					ceilingOffsets[entityID] = collider.pos.y + groundCollider.pos.y + groundCollider.size.y - translation.y;
					hitGround.insert(groundID);
				}
			}
		}
	}
}

void entitystate_t::stepBounceAnimations(entitymap_t<bounceanim_t>& state, float dt) const {
	for (auto& row : state) {
		bounceanim_t& anim = row.second;
		if (anim.elapsed < anim.duration) {
			row.second.elapsed += dt;
		}
	}

	for (entityid_t groundID : pGame->world.entity.hitGround) {
		if (pGame->world.entity.canBounce.find(groundID) != pGame->world.entity.canBounce.end()) {
			bounceanim_t& anim = state[groundID];
			if (anim.elapsed >= anim.duration) {
				anim.duration = BLOCK_BOUNCE_DURATION;
				anim.height = BLOCK_BOUNCE_HEIGHT;
				anim.elapsed = 0;
			}
		}
	}
}

void entitystate_t::stepSpriteOffsets(entitymap_t<glm::vec3>& state) const {
	for (const auto& bounceRow : pGame->world.entity.bounceAnimations) {
		entityid_t id = bounceRow.first;
		const bounceanim_t& anim = bounceRow.second;
		float& yOffset = state[id].y;
		yOffset = 0;
		if (anim.elapsed < anim.duration) {
			yOffset = -glm::sin(glm::pi<float>() * anim.elapsed / anim.duration) * anim.height;
		}
	}
}

void entitystate_t::stepVelocities(entitymap_t<glm::vec3>& velocities) const {
	for (const auto& wallRow : pGame->world.entity.wallOffsets) {
		if (wallRow.second != 0) {
			velocities.find(wallRow.first)->second.x = 0;
		}
	}

	for (const auto& groundRow : pGame->world.entity.groundOffsets) {
		velocities.find(groundRow.first)->second.y = 0;
	}

	for (entityid_t entityID : pGame->world.entity.underGravity) {
		velocities[entityID].y += 10.0f;
	}

	for (const auto& ceilingRow : pGame->world.entity.ceilingOffsets) {
		velocities.find(ceilingRow.first)->second.y = 0;
	}
}

void entitystate_t::stepTranslations(entitymap_t<glm::vec3>& state, float dt) const {
	for (const auto& velocityRow : pGame->world.entity.velocities) {
		state[velocityRow.first] += velocityRow.second * dt;
	}

	for (const auto& wallRow : pGame->world.entity.wallOffsets) {
		state.find(wallRow.first)->second.x += wallRow.second;
	}

	for (const auto& groundRow : pGame->world.entity.groundOffsets) {
		state.find(groundRow.first)->second.y += groundRow.second;
	}

	for (const auto& ceilingRow : pGame->world.entity.ceilingOffsets) {
		state.find(ceilingRow.first)->second.y += ceilingRow.second;
	}
}

void entitystate_t::stepSpriteAnimators(entitymap_t<spriteanimator_t>& state, float dt) const {
	for (auto& row : state) {
		row.second.elapsed += dt;
	}

	for (const auto& animRow : pGame->world.entity.animationsLeft) {
		entityid_t entityID = animRow.first;
		animid_t animID = animRow.second;
		float xVel = getVelocity(entityID, *pGame).x;

		if (xVel < 0) {
			auto& animator = state[entityID];
			if (animator.animation != animID) {
				animator.animation = animRow.second;
				animator.elapsed = 0;
			}
		}
	}

	for (const auto& animRow : pGame->world.entity.animationsRight) {
		entityid_t entityID = animRow.first;
		animid_t animID = animRow.second;
		float xVel = getVelocity(entityID, *pGame).x;

		if (xVel > 0) {
			auto& animator = state[entityID];
			if (animator.animation != animID) {
				animator.animation = animRow.second;
				animator.elapsed = 0;
			}
		}
	}
}

void entitystate_t::stepSprites(entitymap_t<tileid_t>& state) const {
	for (const auto& animatorRow : pGame->world.entity.spriteAnimators) {
		const auto& animator = animatorRow.second;
		animid_t animationID = animator.animation;
		const auto& animation = getAnimation(animationID, *pGame);
		int elapsedMS = animator.elapsed * 1000;
		int clampedElapsed = elapsedMS % animation.totalDuration;
		int acc = 0;
		for (const auto& frame : animation.frames) {
			acc += frame.duration;
			if (clampedElapsed < acc) {
				entityid_t entityID = animatorRow.first;
				state[entityID] = frame.tileid;
				break;
			}
		}
	}

	for (const auto& row : pGame->world.entity.prizeNum) {
		if (row.second <= 0) {
			entityid_t entityID = row.first;
			auto emptyTileIt = pGame->level.objects.emptyTiles.find(levelobjectid_t({ pGame->world.level, entityID.id }));
			if (emptyTileIt != pGame->level.objects.emptyTiles.end()) {
				state[entityID] = emptyTileIt->second;
			}
		}
	}
}

static void stepLifetimes(entitymap_t<float>& state, float dt) {
	for (auto& row : state) {
		row.second -= dt;
	}
}

void entitystate_t::stepBounceNum(entitymap_t<int>& state) const {
	for (entityid_t groundID : pGame->world.entity.hitGround) {
		auto bounceNumIt = state.find(groundID);
		if (bounceNumIt != state.end()) {
			--(bounceNumIt->second);
		}
	}
}

void entitystate_t::stepCanBounce(entityset_t& state) const {
	auto it = state.begin();
	while (it != state.end()) {
		auto bounceNumIt = pGame->world.entity.bounceNum.find(*it);
		if (bounceNumIt != pGame->world.entity.bounceNum.end() && bounceNumIt->second <= 0) {
			it = state.erase(it);
		} else {
			++it;
		}
	}
}

void entitystate_t::stepPrizeNum(entitymap_t<int>& state) const {
	for (entityid_t entityID : pGame->world.entity.hitGround) {
		auto prizeIt = state.find(entityID);
		if (prizeIt != state.end()) {
			--(prizeIt->second);
		}
	}
}

void entitystate_t::stepPrizes(entitymap_t<prize_t>& state) const {
	auto it = state.begin();
	while (it != state.end()) {
		entityid_t entityID = it->first;
		int prizeNum = getPrizeNum(entityID, *pGame);
		if (prizeNum <= 0) {
			it = state.erase(it);
		} else {
			++it;
		}
	}
}

void entitystate_t::step(float dt) {
	stepWallOffsets(wallOffsets, dt);
	stepColliders(groundOffsets, dt);
	stepCeilingOffsets(ceilingOffsets, hitGround, dt);

	stepBounceAnimations(bounceAnimations, dt);
	stepSpriteOffsets(spriteOffsets);

	stepTranslations(translations, dt);
	stepVelocities(velocities);
	stepSpriteAnimators(spriteAnimators, dt);
	stepSprites(tilesetSprites);

	stepCanBounce(canBounce);
	stepBounceNum(bounceNum);

	stepPrizes(prizes);
	stepPrizeNum(prizeNum);

	stepLifetimes(lifetimes, dt);
}

} // namespace te
