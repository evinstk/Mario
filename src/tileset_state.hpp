#pragma once
#include "id.hpp"
#include "types.hpp"
#include <tegl/types.hpp>
#include <EASTL/hash_map.h>
#include <EASTL/vector_map.h>
#include <EASTL/string.h>

namespace te {

struct tilesetstate_t {
	tilesetstate_t() : nextID(1), nextColliderID(1) {}

	int nextID;
	stringmap_t<tilesetid_t> source;

	tilesetmap_t<tileset_t> tileset;
	animmap_t<animation_t> animation;
	stringmap_t<animid_t> animationID;

	int nextColliderID;
	stringmap_t<colliderid_t> colliderID;
	collidermap_t<aabb_t> collider;

	eastl::vector_set<tileid_t> solid;

	stringmap_t<tileid_t> tileID;
};

} // namespace te
