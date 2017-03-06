#pragma once
#include "types.hpp"

namespace te {

struct levelobjectstate_t {
	levelobjectmap_t<glm::vec3>    translations;
	levelobjectmap_t<tileid_t>     tiles;

	levelobjectmap_t<animid_t>     animations;
	levelobjectmap_t<animid_t>     animationsLeft;
	levelobjectmap_t<animid_t>     animationsRight;

	levelobjectmap_t<colliderid_t> colliders;
	levelobjectset_t               gravities;
	levelobjectset_t               grounds;

	levelobjectmap_t<prize_t>      prizes;
	levelobjectmap_t<int>          prizeNum;
	levelobjectmap_t<tileid_t>     emptyTiles;

	levelobjectmap_t<soundid_t>    bounceSounds;
	levelobjectset_t               canBounce;
	levelobjectmap_t<int>          bounceNum;
};

} // namespace te
