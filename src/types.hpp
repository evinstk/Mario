#pragma once

#include "id.hpp"
#include <EASTL/vector.h>
#include <EASTL/vector_map.h>
#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include <EASTL/vector_set.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL.h>
#include <SDL_mixer.h>
#include <memory>

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

struct frame_t {
	tileid_t tileid;
	int duration;
};

struct animation_t {
	int totalDuration;
	eastl::vector<frame_t> frames;
};

struct bounceanim_t {
	float duration;
	float height;
	float elapsed;
};

struct spriteanimator_t {
	animid_t animation;
	float elapsed;
	spriteanimator_t() : animation({ tilesetid_t(0), 0 }), elapsed(0) {}
};

enum class entity_t {
	BLOCK_COIN
};

struct entityrequest_t {
	entity_t type;
	glm::vec3 translation;
};

enum class prize_t {
	COIN,
	FIRE
};

using entityset_t = eastl::vector_set<entityid_t>;

template <typename T>
using entitymap_t = eastl::vector_map<entityid_t, T>;

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
using soundmap_t = eastl::vector_map<soundid_t, T>;

template <typename T>
using musicmap_t = eastl::vector_map<musicid_t, T>;

using musicptr_t = std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>;

template <typename T>
using levelmap_t = eastl::vector_map<levelid_t, T>;

template <typename T>
using levelobjectmap_t = eastl::vector_map<levelobjectid_t, T>;

using levelobjectset_t = eastl::vector_set<levelobjectid_t>;

} // namespace te
