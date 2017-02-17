#include "reducer_world.hpp"
#include "entity_state.hpp"
#include "game_state.hpp"
#include "tileset_state.hpp"
#include "level_state.hpp"
#include "util.hpp"

namespace te {

static int senseOnWall(int y,
					   int xStart,
					   int xEnd,
					   int xSize,
					   int ySize,
					   const layer_t& platformLayer,
					   const eastl::vector_set<tileid_t>& solids) {

	int yTile = y / ySize;

	for (int xTile = xStart / xSize, end = xEnd / xSize; xTile <= end; ++xTile) {

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
		entity_t entityID = colliderRow.first;
		colliderid_t colliderID = colliderRow.second;
		const aabb_t& collider = game.tileset.collider.find(colliderID)->second;
		glm::vec3 velocity = game.world.entity.velocities.find(entityID)->second;
		glm::vec3 translation = game.world.entity.translations.find(entityID)->second + velocity * dt;

		int y      = translation.y + collider.pos.y + (collider.size.y / 2);
		int xStart = translation.x + collider.pos.x;
		int xEnd   = xStart + collider.size.x;

		int xWallPos = senseOnWall(y, xStart, xEnd, tileSize.x, tileSize.y, platformLayer, game.tileset.solid);

		if (xWallPos >= 0) {
			if (translation.x > xWallPos) {
				float offset = xWallPos + tileSize.x - translation.x - collider.pos.x;
				state.insert({ entityID, offset });
			} else {
				float offset = xWallPos - translation.x - collider.pos.x - collider.size.x;
				state.insert({ entityID, offset });
			}
		} else {
			state.insert({ entityID, 0 });
		}
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

	for (int yTile = yStart / ySize, end = yEnd / ySize; yTile <= end; ++yTile) {

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
						  entityset_t& falling,
						  float dt,
						  const gamestate_t& game) {
	groundOffsets.clear();
	falling.clear();

	glm::ivec2 tileSize = getMap(game).tileSize;
	const layer_t& platformLayer = getPlatformLayer(game);

	for (entity_t entityID : game.world.entity.underGravity) {

		glm::vec3 velocity = game.world.entity.velocities.find(entityID)->second;
		if (velocity.y < 0) {
			falling.insert(entityID);
			continue;
		}

		colliderid_t colliderID = game.world.entity.colliders.find(entityID)->second;
		const aabb_t& collider = game.tileset.collider.find(colliderID)->second;
		glm::vec3 translation = getTranslation(entityID, game) + velocity * dt;
		translation.x += game.world.entity.wallOffsets.find(entityID)->second;

		int halfColliderSizeY = collider.size.y / 2;
		int yStart = translation.y + collider.pos.y + halfColliderSizeY;
		int yEnd   = yStart + halfColliderSizeY + 1;
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
			++entityCollider.size.y;
			for (entity_t groundID : game.world.entity.isGround) {
				aabb_t groundCollider = getCollider(groundID, game);
				groundCollider.pos += glm::vec2(getTranslation(groundID, game));
				if (isColliding(entityCollider, groundCollider)) {
					offset = groundCollider.pos.y - translation.y - collider.pos.y - collider.size.y;
					onGround = true;
				}
			}
			if (onGround) {
				groundOffsets.insert({ entityID, offset });
			} else {
				falling.insert(entityID);
			}
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

	for (const entity_t& fallingEntity : game.world.entity.falling) {
		velocities[fallingEntity].y += 10.0f;
	}
}

static void stepTranslations(entitymap_t<glm::vec3>& translations, float dt, const gamestate_t& game) {
	for (const auto& velocityRow : game.world.entity.velocities) {
		translations[velocityRow.first] += velocityRow.second * dt;
	}

	for (const auto& wallRow : game.world.entity.wallOffsets) {
		translations.find(wallRow.first)->second.x += wallRow.second;
	}

	for (const auto& groundRow : game.world.entity.groundOffsets) {
		translations.find(groundRow.first)->second.y += groundRow.second;
	}
}

static void stepAnimators(const animctrlmap_t<animctrl_t>& controllers,
						  const entitymap_t<glm::vec3>& velocities,
						  float dt,
						  entitymap_t<animator_t>& animators) {

	for (auto& animatorRow : animators) {
		animator_t& animator = animatorRow.second;
		animator.elapsed += dt;
		entity_t entityID = animatorRow.first;
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
				entity_t entityID = animatorRow.first;
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

void stepEntity(entitystate_t& state, float dt, const gamestate_t& game) {
	stepWallOffsets(state.wallOffsets, dt, game);
	stepColliders(state.groundOffsets, state.falling, dt, game);
	stepTranslations(state.translations, dt, game);
	stepVelocities(state.velocities, game);
	stepAnimators(game.tileset.controller, state.velocities, dt, state.animators);
	auto levelTilesetsIt = game.level.tilesets.find(game.world.level);
	assert(levelTilesetsIt != game.level.tilesets.end());
	stepSprites(state.animators, game.tileset.animation, state.tilesetSprites, levelTilesetsIt->second);
}

} // namespace te
