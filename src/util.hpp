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

inline void setView(glm::mat4& view, const glm::vec3& playerTranslation) {
	view = glm::translate(glm::ivec3(-playerTranslation.x + 16 * 4,
									 -playerTranslation.y + 16 * 9,
									 0));
}

} // namespace te
