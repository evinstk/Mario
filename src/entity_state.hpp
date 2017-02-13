#pragma once
#include "types.hpp"
#include <EASTL/vector_map.h>
#include <glm/vec3.hpp>

namespace te {

struct entitystate_t {
	entitymap_t<glm::vec3> velocities;
	entitymap_t<glm::vec3> translations;
	entitymap_t<int> tilesetSprites;
	entitymap_t<animator_t> animators;
};

} // namespace te
