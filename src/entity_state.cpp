#include "entity_state.hpp"

namespace te {

static entitystate_t gState;
const entitystate_t& gEntity = gState;

void inputEntity(entitystate_t& entity, const Uint8 *keyboardState);
void inputEntity(const Uint8 *keyboardState) {
	inputEntity(gState, keyboardState);
}

void stepEntity(entitystate_t& entity, float dt);
void stepEntity(float dt) {
	stepEntity(gState, dt);
}

void runEntity(entitystate_t& entity, levelid_t levelID);
void runEntity(levelid_t levelID) {
	runEntity(gState, levelID);
}

void processEntity(entitystate_t& entity, entityid_t playerEntity, const SDL_Event& evt, bool& jump);
void processEntity(const SDL_Event& evt, entityid_t playerEntity, bool& jump) {
	processEntity(gState, playerEntity, evt, jump);
}

void makeEntity(entitystate_t& entity);
void makeEntityEntity() {
	makeEntity(gState);
}

void destroyEntity(entitystate_t& entity);
void destroyEntityEntity() {
	destroyEntity(gState);
}

}
