#include "world_state.hpp"
#include "camera.hpp"
#include "game_values.hpp"
#include <glm/gtx/transform.hpp>

namespace te {

worldstate_t::worldstate_t(const gamestate_t& g)
	: mode(worldmode_t::PLAY)
	, projection(glm::ortho(0.0f, (float)CAMERA_WIDTH, (float)CAMERA_HEIGHT, 0.0f, -100.0f, 100.0f))
	  //, view(glm::translate(glm::vec3(-64, -128, 0)))
	, entity(g)
	, playerEntity(0)
	, score(0)
	, coinCount(0)
	, lives(LIVE_COUNT_START)
	, pGame(&g)
{
}

} // namespace te
