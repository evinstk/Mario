#pragma once
#include "types.hpp"
#include "tileset_state.hpp"
#include "game_state.hpp"
#include "world_state.hpp"
#include "entity_state.hpp"
#include <glm/gtx/transform.hpp>

namespace te {

inline bool isValid(tileid_t tile) {
	return tile.id.first.id > 0;
}

inline const layer_t& getPlatformLayer(const gamestate_t& game) {
	int platformIndex = game.level.platformIndex.find(gWorld.level)->second;
	const layer_t& platformLayer = gWorld.layers[platformIndex];
	return platformLayer;
}

inline const map_t& getMap(const gamestate_t& game) {
	return game.level.map.find(gWorld.level)->second;
}

inline void setView(glm::imat4& view, const glm::vec3& playerTranslation) {
	view = glm::translate(glm::ivec3(-playerTranslation.x + 16 * 4,
									 -playerTranslation.y + 16 * 9,
									 0));
}

inline const glm::vec3& getTranslation(entityid_t entityID) {
	return gEntity.translations.find(entityID)->second;
}

inline const aabb_t& getCollider(entityid_t entityID, const gamestate_t& game) {
	return gTileset.collider.find(gEntity.colliders.find(entityID)->second)->second;
}

inline const glm::vec3& getVelocity(entityid_t entityID) {
	return gEntity.velocities.find(entityID)->second;
}

inline float getWallOffset(entityid_t entityID) {
	return gEntity.wallOffsets.find(entityID)->second;
}

inline const animation_t& getAnimation(animid_t id, const gamestate_t& game) {
	return gTileset.animation.find(id)->second;
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

inline bool hasPrize(entityid_t blockID, prize_t& prize) {
	auto prizeIt = gEntity.prizes.find(blockID);
	if (prizeIt != gEntity.prizes.end()) {
		prize = prizeIt->second;
		return true;
	}
	return false;
}

inline soundid_t getSoundID(const char *pathname, const gamestate_t& game) {
	return game.sound.soundID.find_as(pathname)->second;
}

inline Mix_Chunk *getChunk(soundid_t soundID, const gamestate_t& game) {
	return game.sound.chunk.find(soundID)->second.get();
}

inline musicid_t getMusicID(const char *pathname, const gamestate_t& game) {
	return game.sound.musicID.find_as(pathname)->second;
}

inline Mix_Music *getMusic(musicid_t musicID, const gamestate_t& game) {
	return game.sound.music.find(musicID)->second.get();
}

inline bool canBounce(entityid_t entityID) {
	return gEntity.canBounce.find(entityID) != gEntity.canBounce.end();
}

inline int getPrizeNum(entityid_t entityID) {
	return gEntity.prizeNum.find(entityID)->second;
}

inline tileid_t getTileID(const char *strID, const gamestate_t& game) {
	return gTileset.tileID.find_as(strID)->second;
}

} // namespace te
