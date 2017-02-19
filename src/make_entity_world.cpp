#include "game_action.hpp"
#include "world_state.hpp"

namespace te {

void makeEntity(worldstate_t& state,
				entity_t entity,
				const glm::vec3& translation,
				const gamestate_t& game) {
	entityid_t entityID(state.nextEntityID++);
	makeEntity(state.entity, entityID, entity, translation, game);
}

} // namespace te
