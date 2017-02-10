#include "world_state.hpp"
#include <glm/gtx/transform.hpp>

namespace te {

worldstate_t::worldstate_t()
	: projection(glm::ortho(0.0f, 320.0f, 180.0f, 0.0f, -100.0f, 100.0f))
{
}

} // namespace te
