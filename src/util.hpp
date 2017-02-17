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
	return game.world.entity.translations.find(entityID)->second;
}

inline const aabb_t& getCollider(entity_t entityID, const gamestate_t& game) {
	return game.tileset.collider.find(game.world.entity.colliders.find(entityID)->second)->second;
}

inline const glm::vec3& getVelocity(entity_t entityID, const gamestate_t& game) {
	return game.world.entity.velocities.find(entityID)->second;
}

inline float getWallOffset(entity_t entityID, const gamestate_t& game) {
	return game.world.entity.wallOffsets.find(entityID)->second;
}

inline bool isColliding(const aabb_t& aabb1, const aabb_t& aabb2) {
	return ( aabb1.pos.x < aabb2.pos.x + aabb2.size.x &&
			 aabb1.pos.x + aabb1.size.x > aabb2.pos.x &&
			 aabb1.pos.y < aabb2.pos.y + aabb2.size.y &&
			 aabb1.pos.y + aabb1.size.y > aabb2.pos.y );
}

template <typename Container, typename Key>
inline eastl::pair<typename Container::const_iterator, typename Container::const_iterator> parentRange(const Container& container, const Key& parentKey) {
	return {
		container.lower_bound(typename Container::key_type({ parentKey, 0 })),
		container.lower_bound(typename Container::key_type({ parentKey.next(), 0 })),
	};
}

} // namespace te
