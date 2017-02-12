#pragma once
#include "id.hpp"
#include <tegl/types.hpp>
#include <EASTL/vector_map.h>
#include <EASTL/vector.h>
#include <glm/glm.hpp>
#include <GL/glew.h>

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

template <typename T>
using entitymap_t = eastl::vector_map<entity_t, T>;

template <typename T>
using animmap_t = eastl::vector_map<animid_t, T>;

template <typename T>
using animctrlmap_t = eastl::vector_map<animctrlid_t, T>;

template <typename T>
using collidermap_t = eastl::vector_map<colliderid_t, T>;

template <typename T>
using vector_t = eastl::vector<T>;

struct worldstate_t {
	worldstate_t();

	glm::mat4 projection;
	glm::mat4 view;

	entitymap_t<glm::vec3> velocities;
	entitymap_t<glm::vec3> translations;
	entitymap_t<int> tilesetSprites;
	entitymap_t<animator_t> animators;

	map_t map;
	vector_t<tileset_t> tilesets;
	vector_t<layer_t> layers;

	animmap_t<animation_t> animations;
	animctrlmap_t<animctrl_t> animationControllers;
	collidermap_t<aabb_t> colliders;

	entity_t playerEntity;
};

} // namespace te
