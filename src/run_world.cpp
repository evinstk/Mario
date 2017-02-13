#include "reducer_world.hpp"
#include "game_state.hpp"
#include <cassert>

namespace te {

void runWorld(worldstate_t& state, levelid_t levelID, const levelstate_t& levelState) {
	state.level = levelID;
	auto layersIt = levelState.layers.find(levelID);
	assert(layersIt != levelState.layers.end());
	state.layers = layersIt->second;
}

}
