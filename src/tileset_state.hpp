#pragma once
#include "id.hpp"
#include "types.hpp"
#include <EASTL/hash_map.h>
#include <EASTL/vector_map.h>
#include <EASTL/string.h>

namespace te {

struct tilesetstate_t {
	tilesetstate_t() : nextID(1), nextControllerID(1) {}

	int nextID;
	stringmap_t<tilesetid_t> source;

	tilesetmap_t<tileset_t> tileset;
	animmap2_t<animation2_t> animation;

	int nextControllerID;
	stringmap_t<animctrlid2_t> controllerID;
	animctrlmap2_t<animctrl2_t> controller;
};

} // namespace te
