#pragma once
#include "types.hpp"
#include "game_state.hpp"
#include <glm/gtx/transform.hpp>

namespace te {

inline bool isValid(tileid_t tile) {
	return tile.id.first.id > 0;
}

inline const layer_t& getPlatformLayer(const gamestate_t& game) {
	int platformIndex = game.level.platformIndex.find(game.world.level)->second;
	const layer_t& platformLayer = game.world.layers[platformIndex];
	return platformLayer;
}

inline const map_t& getMap(const gamestate_t& game) {
	return game.level.map.find(game.world.level)->second;
}

inline void setView(glm::imat4& view, const glm::vec3& playerTranslation) {
	view = glm::translate(glm::ivec3(-playerTranslation.x + 16 * 4,
									 -playerTranslation.y + 16 * 9,
									 0));
}

inline const glm::vec3& getTranslation(entity_t entityID, const gamestate_t& game) {
	auto it = game.world.entity.translations.find(entityID);
	assert(it != game.world.entity.translations.end());
	return it->second;
}

inline const aabb_t& getCollider(entity_t entityID, const gamestate_t& game) {
	auto it = game.world.entity.colliders.find(entityID);
	assert(it != game.world.entity.colliders.end());
	auto colliderIt = game.tileset.collider.find(it->second);
	assert(colliderIt != game.tileset.collider.end());
	return colliderIt->second;
}

inline bool isColliding(const aabb_t& aabb1, const aabb_t& aabb2) {
	return ( aabb1.pos.x < aabb2.pos.x + aabb2.size.x &&
			 aabb1.pos.x + aabb1.size.x > aabb2.pos.x &&
			 aabb1.pos.y < aabb2.pos.y + aabb2.size.y &&
			 aabb1.pos.y + aabb1.size.y > aabb2.pos.y );
}

} // namespace te
