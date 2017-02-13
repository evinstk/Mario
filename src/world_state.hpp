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

	levelid_t level;
	vector_t<layer_t> layers;

	entity_t playerEntity;
};

} // namespace te
