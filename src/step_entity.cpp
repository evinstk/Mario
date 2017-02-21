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

static void stepWallOffsets(entitymap_t<float>& state, float dt, const gamestate_t& game) {
	state.clear();

	const layer_t& platformLayer = getPlatformLayer(game);
	glm::vec2 tileSize = getMap(game).tileSize;

	for (const auto& colliderRow : game.world.entity.colliders) {
		entityid_t entityID = colliderRow.first;
		colliderid_t colliderID = colliderRow.second;
		const aabb_t& collider = game.tileset.collider.find(colliderID)->second;
		glm::vec3 velocity = game.world.entity.velocities.find(entityID)->second;
		glm::vec3 translation = game.world.entity.translations.find(entityID)->second + velocity * dt;

		glm::vec2 halfSize = collider.size / glm::vec2(2, 2);
		glm::vec2 start = glm::vec2(translation) + collider.pos + halfSize;
		glm::vec2 end1  = start - glm::vec2(halfSize.x, 0);
		glm::vec2 end2  = start + glm::vec2(halfSize.x, 0);

		int xWallPos1 = senseOnWall(start.y, start.x, end1.x, tileSize.x, tileSize.y, platformLayer, game.tileset.solid);
		int xWallPos2 = senseOnWall(start.y, start.x, end2.x, tileSize.x, tileSize.y, platformLayer, game.tileset.solid);

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
			for (entityid_t groundID : game.world.entity.isGround) {
				aabb_t groundCollider = getCollider(groundID, game);
				groundCollider.pos += glm::vec2(getTranslation(groundID, game));
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

static void stepColliders(entitymap_t<float>& groundOffsets,
						  float dt,
						  const gamestate_t& game) {
	groundOffsets.clear();

	glm::ivec2 tileSize = getMap(game).tileSize;
	const layer_t& platformLayer = getPlatformLayer(game);

	for (entityid_t entityID : game.world.entity.underGravity) {

		glm::vec3 velocity = game.world.entity.velocities.find(entityID)->second;
		if (velocity.y < 0) {
			continue;
		}

		colliderid_t colliderID = game.world.entity.colliders.find(entityID)->second;
		const aabb_t& collider = game.tileset.collider.find(colliderID)->second;
		glm::vec3 translation = getTranslation(entityID, game) + velocity * dt;
		translation.x += game.world.entity.wallOffsets.find(entityID)->second;

		int halfColliderSizeY = collider.size.y / 2;
		int yStart = translation.y + collider.pos.y + halfColliderSizeY;
		int yEnd   = yStart + halfColliderSizeY;
		int x1     = translation.x + collider.pos.x + 1;
		int x2     = translation.x + collider.pos.x + collider.size.x - 1;

		int solidTile1 = senseGround(x1, yStart, yEnd, tileSize.x, tileSize.y, platformLayer, game.tileset.solid);
		int solidTile2 = senseGround(x2, yStart, yEnd, tileSize.x, tileSize.y, platformLayer, game.tileset.solid);

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
			for (entityid_t groundID : game.world.entity.isGround) {
				aabb_t groundCollider = getCollider(groundID, game);
				groundCollider.pos += glm::vec2(getTranslation(groundID, game));
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

static void stepCeilingOffsets(entitymap_t<float>& ceilingOffsets,
							   entityset_t& hitGround,
							   float dt,
							   const gamestate_t& game) {
	ceilingOffsets.clear();
	hitGround.clear();

	for (entityid_t entityID : game.world.entity.underGravity) {
		glm::vec2 velocity = getVelocity(entityID, game);
		if (velocity.y >= 0) {
			continue;
		}

		aabb_t collider = getCollider(entityID, game);
		float halfColliderSizeY = collider.size.y * 0.5f;
		glm::vec2 translation = glm::vec2(getTranslation(entityID, game)) + velocity * dt;
		translation.x += getWallOffset(entityID, game);
		glm::vec2 tileSize = getMap(game).tileSize;
		const layer_t& platformLayer = getPlatformLayer(game);

		glm::vec2 start1 = translation + collider.pos + glm::vec2(2.0f, halfColliderSizeY);
		glm::vec2 end1   = translation + collider.pos + glm::vec2(2.0f, 0.0f);
		glm::vec2 start2 = translation + collider.pos + glm::vec2(collider.size.x - 3.0f, halfColliderSizeY);
		glm::vec2 end2   = translation + collider.pos + glm::vec2(collider.size.x - 3.0f, 0.0f);

		int solidTile1 = senseGround(start1.x, start1.y, end1.y, tileSize.x, tileSize.y, platformLayer, game.tileset.solid);
		int solidTile2 = senseGround(start2.x, start2.y, end2.y, tileSize.x, tileSize.y, platformLayer, game.tileset.solid);

		if (solidTile1 >= 0) {
			ceilingOffsets.insert({ entityID, solidTile1 + tileSize.y - translation.y - collider.pos.y });
		} else if (solidTile2 >= 0) {
			ceilingOffsets.insert({ entityID, solidTile2 + tileSize.y - translation.y - collider.pos.y });
		} else {
			aabb_t entityCollider = collider;
			entityCollider.pos += translation;
			entityCollider.size.y /= 2;
			for (entityid_t groundID : game.world.entity.isGround) {
				glm::vec2 groundTranslation = getTranslation(groundID, game);
				aabb_t groundCollider = getCollider(groundID, game);
				groundCollider.pos += groundTranslation;
				if (isColliding(entityCollider, groundCollider)) {
					ceilingOffsets[entityID] = collider.pos.y + groundCollider.pos.y + groundCollider.size.y - translation.y;
					hitGround.insert(groundID);
				}
			}
		}
	}
}

static void stepBounceAnimations(entitymap_t<bounceanim_t>& state, float dt, const gamestate_t& game) {
	for (auto& row : state) {
		bounceanim_t& anim = row.second;
		if (anim.elapsed < anim.duration) {
			row.second.elapsed += dt;
		}
	}

	for (entityid_t groundID : game.world.entity.hitGround) {
		if (game.world.entity.canBounce.find(groundID) != game.world.entity.canBounce.end()) {
			bounceanim_t& anim = state[groundID];
			if (anim.elapsed >= anim.duration) {
				anim.duration = BLOCK_BOUNCE_DURATION;
				anim.height = BLOCK_BOUNCE_HEIGHT;
				anim.elapsed = 0;
			}
		}
	}
}

static void stepSpriteOffsets(entitymap_t<glm::vec3>& state, const gamestate_t& game) {
	for (const auto& bounceRow : game.world.entity.bounceAnimations) {
		entityid_t id = bounceRow.first;
		const bounceanim_t& anim = bounceRow.second;
		float& yOffset = state[id].y;
		yOffset = 0;
		if (anim.elapsed < anim.duration) {
			yOffset = -glm::sin(glm::pi<float>() * anim.elapsed / anim.duration) * anim.height;
		}
	}
}

static void stepVelocities(entitymap_t<glm::vec3>& velocities, const gamestate_t& game) {
	for (const auto& wallRow : game.world.entity.wallOffsets) {
		if (wallRow.second != 0) {
			velocities.find(wallRow.first)->second.x = 0;
		}
	}

	for (const auto& groundRow : game.world.entity.groundOffsets) {
		velocities.find(groundRow.first)->second.y = 0;
	}

	for (entityid_t entityID : game.world.entity.underGravity) {
		velocities[entityID].y += 10.0f;
	}

	for (const auto& ceilingRow : game.world.entity.ceilingOffsets) {
		velocities.find(ceilingRow.first)->second.y = 0;
	}
}

static void stepTranslations(entitymap_t<glm::vec3>& state, float dt, const gamestate_t& game) {
	for (const auto& velocityRow : game.world.entity.velocities) {
		state[velocityRow.first] += velocityRow.second * dt;
	}

	for (const auto& wallRow : game.world.entity.wallOffsets) {
		state.find(wallRow.first)->second.x += wallRow.second;
	}

	for (const auto& groundRow : game.world.entity.groundOffsets) {
		state.find(groundRow.first)->second.y += groundRow.second;
	}

	for (const auto& ceilingRow : game.world.entity.ceilingOffsets) {
		state.find(ceilingRow.first)->second.y += ceilingRow.second;
	}
}

static void stepAnimators(const animctrlmap_t<animctrl_t>& controllers,
						  const entitymap_t<glm::vec3>& velocities,
						  float dt,
						  entitymap_t<animator_t>& animators) {

	for (auto& animatorRow : animators) {
		animator_t& animator = animatorRow.second;
		animator.elapsed += dt;
		entityid_t entityID = animatorRow.first;
		glm::vec3 velocity = velocities.find(entityID)->second;
		const animctrl_t& controller = controllers.find(animator.controller)->second;
		animid_t newAnim;
		if (velocity.x > 0) {
			newAnim = controller.walkRight;
		} else if (velocity.x < 0) {
			newAnim = controller.walkLeft;
		}
		if (newAnim != animator.animation && newAnim.id.second > 0) {
			animator.animation = newAnim;
			animator.elapsed = 0;
		}
	}
}

static void stepSprites(const entitymap_t<animator_t>& animators,
						const animmap_t<animation_t>& animations,
						entitymap_t<tileid_t>& sprites,
						const vector_t<leveltileset_t>& levelTilesets) {
	for (const auto& animatorRow : animators) {
		const auto& animator = animatorRow.second;
		if (animator.animation.id.second == 0) {
			continue;
		}
		animid_t animationID = animator.animation;
		const auto& animation = animations.find(animationID)->second;
		int elapsedMS = animator.elapsed * 1000;
		int clampedElapsed = elapsedMS % animation.totalDuration;
		int acc = 0;
		for (const auto& frame : animation.frames) {
			acc += frame.duration;
			if (clampedElapsed < acc) {
				entityid_t entityID = animatorRow.first;
				tilesetid_t tilesetID = animationID.id.first;
				auto tilesetIt = eastl::find_if(levelTilesets.begin(), levelTilesets.end(), [tilesetID](const auto& ltileset) {
						return ltileset.tileset == tilesetID;
					});
				assert(tilesetIt != levelTilesets.end());
				sprites[entityID] = tileid_t({ tilesetIt->tileset, frame.tileid });
				break;
			}
		}
	}
}

static void stepLifetimes(entitymap_t<float>& state, float dt) {
	for (auto& row : state) {
		row.second -= dt;
	}
}

static void stepBounceNum(entitymap_t<int>& state, const gamestate_t& game) {
	for (entityid_t groundID : game.world.entity.hitGround) {
		auto bounceNumIt = state.find(groundID);
		if (bounceNumIt != state.end()) {
			--(bounceNumIt->second);
		}
	}
}

static void stepCanBounce(entityset_t& state, const gamestate_t& game) {
	auto it = state.begin();
	while (it != state.end()) {
		auto bounceNumIt = game.world.entity.bounceNum.find(*it);
		if (bounceNumIt != game.world.entity.bounceNum.end() && bounceNumIt->second <= 0) {
			it = state.erase(it);
		} else {
			++it;
		}
	}
}

static void stepBounceSounds(entitymap_t<soundid_t>& state, const gamestate_t& game) {
	auto it = state.begin();
	while (it != state.end()) {
		if (game.world.entity.canBounce.find(it->first) == game.world.entity.canBounce.end()) {
			it = state.erase(it);
		} else {
			++it;
		}
	}
}

void stepEntity(entitystate_t& state, float dt, const gamestate_t& game) {
	stepWallOffsets(state.wallOffsets, dt, game);
	stepColliders(state.groundOffsets, dt, game);
	stepCeilingOffsets(state.ceilingOffsets, state.hitGround, dt, game);

	stepBounceAnimations(state.bounceAnimations, dt, game);
	stepSpriteOffsets(state.spriteOffsets, game);

	stepTranslations(state.translations, dt, game);
	stepVelocities(state.velocities, game);
	stepAnimators(game.tileset.controller, state.velocities, dt, state.animators);
	auto levelTilesetsIt = game.level.tilesets.find(game.world.level);
	assert(levelTilesetsIt != game.level.tilesets.end());
	stepSprites(state.animators, game.tileset.animation, state.tilesetSprites, levelTilesetsIt->second);

	stepBounceSounds(state.bounceSounds, game);
	stepBounceNum(state.bounceNum, game);
	stepCanBounce(state.canBounce, game);

	stepLifetimes(state.lifetimes, dt);
}

} // namespace te
