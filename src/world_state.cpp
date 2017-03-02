#include "world_state.hpp"
#include "camera.hpp"
#include "game_values.hpp"
#include <glm/gtx/transform.hpp>

namespace te {

worldstate_t::worldstate_t()
	: mode(worldmode_t::PLAY)
	, projection(glm::ortho(0.0f, (float)CAMERA_WIDTH, (float)CAMERA_HEIGHT, 0.0f, -100.0f, 100.0f))
	  //, view(glm::translate(glm::vec3(-64, -128, 0)))
	, playerEntity(0)
	, score(0)
	, coinCount(0)
	, lives(LIVE_COUNT_START)
	, deathTrigger(false)
{
}

static worldstate_t gState;
const worldstate_t& gWorld = gState;

void runWorld(worldstate_t& state, levelid_t levelID);
void runWorld(levelid_t levelID) {
	runWorld(gState, levelID);
}

void processWorld(worldstate_t& state, const SDL_Event& evt);
void processWorld(const SDL_Event& evt) {
	processWorld(gState, evt);
}

void inputWorld(worldstate_t& state, const Uint8 *keyboardState);
void inputWorld(const Uint8 *keyboardState) {
	inputWorld(gState, keyboardState);
}

void stepWorld(worldstate_t& state, float dt);
void stepWorld(float dt) {
	stepWorld(gState, dt);
}

void makeEntityWorld(worldstate_t& state);
void makeEntityWorld() {
	makeEntityWorld(gState);
}

void destroyEntityWorld(worldstate_t& state);
void destroyEntityWorld() {
	destroyEntityWorld(gState);
}

void flushSoundQueue(worldstate_t& state);
void flushSoundQueue() {
	flushSoundQueue(gState);
}

} // namespace te
