#include "entity_state.hpp"
#include "game_state.hpp"

namespace te {

static entitystate_t gState;
const entitystate_t& gEntity = gState;

void inputEntity(entitystate_t& entity, const Uint8 *keyboardState, const gamestate_t& game);
void inputEntity(const Uint8 *keyboardState, const gamestate_t& game) {
	inputEntity(gState, keyboardState, game);
}

void stepEntity(entitystate_t& entity, float dt, const gamestate_t& game);
void stepEntity(float dt, const gamestate_t& game) {
	stepEntity(gState, dt, game);
}

void runEntity(entitystate_t& entity, levelid_t levelID);
void runEntity(levelid_t levelID) {
	runEntity(gState, levelID);
}

void processEntity(entitystate_t& entity, entityid_t playerEntity, const SDL_Event& evt, bool& jump);
void processEntity(const SDL_Event& evt, entityid_t playerEntity, bool& jump) {
	processEntity(gState, playerEntity, evt, jump);
}

void makeEntity(entitystate_t& entity, const gamestate_t& game);
void makeEntity(const gamestate_t& game) {
	makeEntity(gState, game);
}

void destroyEntity(entitystate_t& entity);
void destroyEntity() {
	destroyEntity(gState);
}

}
