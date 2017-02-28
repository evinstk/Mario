#include "entity_state.hpp"
#include "game_state.hpp"

namespace te {

void processEntity(entitystate_t& state,
				   entityid_t playerEntity,
				   const SDL_Event& evt,
				   const gamestate_t& game,
				   bool& jump) {
	static constexpr float JUMP_SPEED = -10 * 32;

	if (game.world.mode != worldmode_t::PLAY) {
		return;
	}

	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_SPACE &&
			evt.key.repeat == 0 &&
			gEntity.groundOffsets.find(playerEntity) != gEntity.groundOffsets.end()) {

			state.velocities[playerEntity].y = JUMP_SPEED;
			jump = true;
			//state.soundQueue.push_back(getSoundID(JUMP_SOUND, game));
		}
	}
}

} // namespace te
