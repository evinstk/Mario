#pragma once
#include "types.hpp"

namespace te {

struct levelobjectstate_t {
	levelobjectmap_t<glm::vec3>    translations;
	levelobjectmap_t<tileid_t>     tiles;
	levelobjectmap_t<animctrlid_t> animationControllers;
	levelobjectmap_t<colliderid_t> colliders;
	levelobjectset_t               gravities;
	levelobjectset_t               grounds;
	levelobjectmap_t<prize_t>      prizes;
};

} // namespace te