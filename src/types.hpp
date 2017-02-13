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
	int firstgid;
	int tilecount;
};

struct layer_t {
	eastl::vector<int> gids;
	int layerIndex;
	glm::ivec2 size;
};

struct frame_t {
	int gid;
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

struct mapstrdata_t {
	eastl::hash_map<eastl::string, animctrlid_t> ctrlMap;
	eastl::hash_map<eastl::string, colliderid_t> colliderMap;
};

struct frame2_t {
	int tileid;
	int duration;
};

struct animation2_t {
	int totalDuration;
	eastl::vector<frame2_t> frames;
};

struct animctrl2_t {
	animid2_t walkLeft;
	animid2_t walkRight;
};

template <typename T>
using entitymap_t = eastl::vector_map<entity_t, T>;

template <typename T>
using collidermap_t = eastl::vector_map<colliderid_t, T>;

template <typename T>
using animmap_t = eastl::vector_map<animid_t, T>;

template <typename T>
using animctrlmap_t = eastl::vector_map<animctrlid_t, T>;

template <typename T>
using vector_t = eastl::vector<T>;

template <typename T>
using stringmap_t = eastl::hash_map<eastl::string, T>;

template <typename T>
using animmap2_t = eastl::vector_map<animid2_t, T>;

template <typename T>
using animctrlmap2_t = eastl::vector_map<animctrlid2_t, T>;

} // namespace te
