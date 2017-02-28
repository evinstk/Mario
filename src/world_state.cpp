#include "world_state.hpp"
#include "camera.hpp"
#include "game_values.hpp"
#include "game_state.hpp"
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
{
}

static worldstate_t gState;
const worldstate_t& gWorld = gState;

void runWorld(worldstate_t& state, levelid_t levelID, const levelstate_t& level);
void runWorld(levelid_t levelID, const levelstate_t& level) {
	runWorld(gState, levelID, level);
}

void processWorld(worldstate_t& state, const SDL_Event& evt, const gamestate_t& game);
void processWorld(const SDL_Event& evt, const gamestate_t& game) {
	processWorld(gState, evt, game);
}

void inputWorld(worldstate_t& state, const Uint8 *keyboardState, const gamestate_t& game);
void inputWorld(const Uint8 *keyboardState, const gamestate_t& game) {
	inputWorld(gState, keyboardState, game);
}

void stepWorld(worldstate_t& state, float dt, const gamestate_t& game);
void stepWorld(float dt, const gamestate_t& game) {
	stepWorld(gState, dt, game);
}

void makeEntityWorld(worldstate_t& state, const gamestate_t& game);
void makeEntityWorld(const gamestate_t& game) {
	makeEntityWorld(gState, game);
}

void destroyEntityWorld(worldstate_t& state, const gamestate_t& game);
void destroyEntityWorld(const gamestate_t& game) {
	destroyEntityWorld(gState, game);
}

void flushSoundQueue(worldstate_t& state);
void flushSoundQueue() {
	flushSoundQueue(gState);
}

} // namespace te
