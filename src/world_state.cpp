#include "world_state.hpp"
#include "camera.hpp"
#include <glm/gtx/transform.hpp>

namespace te {

worldstate_t::worldstate_t()
	: projection(glm::ortho(0.0f, (float)CAMERA_WIDTH, (float)CAMERA_HEIGHT, 0.0f, -100.0f, 100.0f))
	  //, view(glm::translate(glm::vec3(-64, -128, 0)))
	, playerEntity(0)
{
}

} // namespace te
