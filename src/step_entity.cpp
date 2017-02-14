#include "reducer_world.hpp"
#include "entity_state.hpp"
#include "game_state.hpp"
#include "tileset_state.hpp"
#include "level_state.hpp"
#include "util.hpp"

namespace te {

static int senseSolid(int x,
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

static void stepColliders(entitymap_t<int>& grounded,
						  entityset_t& falling,
						  const gamestate_t& game) {
	grounded.clear();
	falling.clear();

	glm::ivec2 tileSize = game.level.map.find(game.world.level)->second.tileSize;
	int platformIndex = game.level.platformIndex.find(game.world.level)->second;
	const layer_t& platformLayer = game.world.layers[platformIndex];

	for (const auto& entityColliderRow : game.world.entity.colliders) {
		entity_t entityID = entityColliderRow.first;

		if (game.world.entity.velocities.find(entityID)->second.y < 0) {
			falling.insert(entityID);
			continue;
		}

		colliderid_t colliderID = entityColliderRow.second;

		const aabb_t& collider = game.tileset.collider.find(colliderID)->second;
		glm::vec3 translation = game.world.entity.translations.find(entityID)->second;

		int halfColliderSizeY = collider.size.y / 2;
		int yStart = translation.y + collider.pos.y + halfColliderSizeY;
		int yEnd   = yStart + halfColliderSizeY + 1;
		int x1     = translation.x + collider.pos.x + 1;
		int x2     = translation.x + collider.pos.x + collider.size.x - 1;

		int solidTile1 = senseSolid(x1, yStart, yEnd, tileSize.x, tileSize.y, platformLayer, game.tileset.solid);
		int solidTile2 = senseSolid(x2, yStart, yEnd, tileSize.x, tileSize.y, platformLayer, game.tileset.solid);

		if (solidTile1 >= 0) {
			grounded.insert({ entityID, solidTile1 });
		} else if (solidTile2 >= 0) {
			grounded.insert({ entityID, solidTile2 });
		} else {
			falling.insert(entityID);
		}
	}
}

static void stepVelocities(entitymap_t<glm::vec3>& velocities, const Uint8 *keyboard, const gamestate_t& game) {
	static constexpr float SPEED = 3 * 32.0f;
	velocities[game.world.playerEntity].x = (keyboard[SDL_SCANCODE_D] - keyboard[SDL_SCANCODE_A]) * SPEED;

	for (const auto& groundedRow : game.world.entity.grounded) {
		entity_t entityID = groundedRow.first;
		velocities.find(entityID)->second.y = 0;
	}

	for (const entity_t& fallingEntity : game.world.entity.falling) {
		velocities[fallingEntity].y += 6.5f;
	}
}

static void stepTranslations(entitymap_t<glm::vec3>& translations, float dt, const gamestate_t& game) {
	for (const auto& velocityRow : game.world.entity.velocities) {
		translations[velocityRow.first] += velocityRow.second * dt;
	}

	for (const auto& groundedRow : game.world.entity.grounded) {
		entity_t entityID = groundedRow.first;
		int yGround = groundedRow.second;
		colliderid_t colliderID = game.world.entity.colliders.find(entityID)->second;
		const aabb_t& collider = game.tileset.collider.find(colliderID)->second;

		translations[entityID].y = yGround - collider.pos.y - collider.size.y;
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

void stepEntity(entitystate_t& state, float dt, const Uint8 *keyboardState, const gamestate_t& game) {
	stepColliders(state.grounded, state.falling, game);
	stepVelocities(state.velocities, keyboardState, game);
	stepTranslations(state.translations, dt, game);
	stepAnimators(game.tileset.controller, state.velocities, dt, state.animators);
	auto levelTilesetsIt = game.level.tilesets.find(game.world.level);
	assert(levelTilesetsIt != game.level.tilesets.end());
	stepSprites(state.animators, game.tileset.animation, state.tilesetSprites, levelTilesetsIt->second);
}

} // namespace te
