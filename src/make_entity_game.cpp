#include "game_action.hpp"
#include "game_state.hpp"

namespace te {

void makeEntity(gamestate_t& state, entity_t entity, const glm::vec3& translation) {
	makeEntity(state.world, entity, translation, state);
}

} // namespace te
