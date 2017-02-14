#pragma once

#include "id.hpp"
#include <EASTL/vector.h>
#include <EASTL/vector_map.h>
#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace te {

struct map_t {
	glm::ivec2 size;
	glm::ivec2 tileSize;
};

struct tileset_t {
	GLuint texture;
	glm::ivec2 tileSize;
	int spacing;
	int margin;
	int columns;
	int tilecount;
};

struct leveltileset_t {
	tilesetid_t tileset;
};

struct layer_t {
	eastl::vector<tileid_t> tiles;
	int layerIndex;
	glm::ivec2 size;
};

struct levelobject_t {
	glm::vec3 translation;
	tileid_t tile;
	animctrlid_t animationController;
};

struct frame_t {
	int tileid;
	int duration;
};

struct animation_t {
	int totalDuration;
	eastl::vector<frame_t> frames;
};

struct animctrl_t {
	animid_t walkLeft;
	animid_t walkRight;
};

struct animator_t {
	animctrlid_t controller;
	animid_t animation;
	float elapsed;
};

template <typename T>
using entitymap_t = eastl::vector_map<entity_t, T>;

template <typename T>
using collidermap_t = eastl::vector_map<colliderid_t, T>;

template <typename T>
using vector_t = eastl::vector<T>;

template <typename T>
using stringmap_t = eastl::hash_map<eastl::string, T>;

template <typename T>
using tilesetmap_t = eastl::vector_map<tilesetid_t, T>;

template <typename T>
using animmap_t = eastl::vector_map<animid_t, T>;

template <typename T>
using animctrlmap_t = eastl::vector_map<animctrlid_t, T>;

template <typename T>
using levelmap_t = eastl::vector_map<levelid_t, T>;

template <typename T>
using levelobjectmap_t = eastl::vector_map<levelobjectid_t, T>;

} // namespace te
