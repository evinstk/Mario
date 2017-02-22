#pragma once
#include "types.hpp"
#include <EASTL/vector_map.h>
#include <glm/vec3.hpp>

namespace te {

struct entitystate_t {
	entitymap_t<colliderid_t> colliders;
	entitymap_t<float> wallOffsets;
	entitymap_t<float> groundOffsets;
	entitymap_t<float> ceilingOffsets;

	entityset_t               hitGround;
	entitymap_t<bounceanim_t> bounceAnimations;
	entitymap_t<glm::vec3>    spriteOffsets;

	entitymap_t<soundid_t> bounceSounds;

	entityset_t underGravity;
	entityset_t isGround;

	entitymap_t<prize_t> prizes;
	entitymap_t<int> prizeNum;

	entitymap_t<glm::vec3> velocities;
	entitymap_t<glm::vec3> translations;
	entitymap_t<tileid_t> tilesetSprites;
	entitymap_t<animator_t> animators;

	entityset_t canBounce;
	entitymap_t<int> bounceNum;

	entitymap_t<float> lifetimes;
};

} // namespace te
