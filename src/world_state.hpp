#pragma once
#include <EASTL/vector_map.h>
#include <EASTL/vector.h>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace te {

enum class id_type_t {
	ENTITY
};

template <id_type_t I, typename T = int>
struct id_t {
	T id;
	bool operator<( const id_t& rhs ) const noexcept {
		return id < rhs.id;
	}
	bool operator==( const id_t& rhs ) const noexcept {
		return id == rhs.id;
	}
};

using entity_t = id_t<id_type_t::ENTITY>;

struct tileset_t {
	GLuint texture;
};

struct layer_t {
	eastl::vector<int> gids;
	int layerIndex;
};

struct worldstate_t {
	eastl::vector_map<entity_t, glm::vec3> velocities;
	eastl::vector_map<entity_t, glm::vec3> translations;

	eastl::vector<tileset_t> tilesets;
	eastl::vector<layer_t> layers;
};

} // namespace te
