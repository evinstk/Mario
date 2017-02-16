#pragma once
#include "types.hpp"
#include <EASTL/vector_map.h>
#include <glm/vec3.hpp>

namespace te {

struct entitystate_t {
	entitymap_t<colliderid_t> colliders;
	entitymap_t<float> wallOffsets;
	entitymap_t<float> groundOffsets;
	entityset_t falling;

	entityset_t underGravity;
	entitymap_t<glm::vec3> velocities;
	entitymap_t<glm::vec3> translations;
	entitymap_t<tileid_t> tilesetSprites;
	entitymap_t<animator_t> animators;
};

} // namespace te
