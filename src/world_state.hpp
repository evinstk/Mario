#pragma once
#include "id.hpp"
#include "types.hpp"
#include "entity_state.hpp"
#include <tegl/types.hpp>
#include <EASTL/vector_map.h>
#include <EASTL/vector.h>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace te {

struct worldstate_t {
	worldstate_t();

	glm::mat4 projection;
	glm::mat4 view;

	entitystate_t entity;

	map_t map;
	vector_t<tileset_t> tilesets;
	vector_t<layer_t> layers;

	animmap_t<animation_t> animations;
	animctrlmap_t<animctrl_t> animationControllers;
	collidermap_t<aabb_t> colliders;

	entity_t playerEntity;
};

} // namespace te
