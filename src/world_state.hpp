#pragma once
#include "id.hpp"
#include "types.hpp"
#include "entity_state.hpp"
#include <tegl/types.hpp>
#include <EASTL/vector_map.h>
#include <EASTL/vector.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_integer.hpp>
#include <GL/glew.h>

namespace te {

struct worldstate_t {
	worldstate_t();

	glm::mat4 projection;
	glm::imat4 view;

	entitystate_t entity;
	int nextEntityID;

	levelid_t level;
	vector_t<layer_t> layers;

	entityid_t playerEntity;
	int score;
	int coinCount;
	int lives;

	vector_t<entityrequest_t> newEntityQueue;
	vector_t<entityid_t> newEntityIDs;
};

} // namespace te
