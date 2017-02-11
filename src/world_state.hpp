#pragma once
#include <EASTL/vector_map.h>
#include <EASTL/vector.h>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace te {

enum class id_type_t {
	ENTITY,
	ANIMATION,
	ANIMATION_CONTROLLER
};

template <id_type_t I, typename T = int>
struct id_t {
	T id;
	id_t() : id( 0 ) {}
	explicit id_t( T _id ) : id( _id ) {}
	bool operator<( const id_t& rhs ) const noexcept {
		return id < rhs.id;
	}
	bool operator==( const id_t& rhs ) const noexcept {
		return id == rhs.id;
	}
	bool operator!=( const id_t& rhs ) const noexcept {
		return !( id == rhs.id );
	}
};

using entity_t = id_t<id_type_t::ENTITY>;
using animid_t = id_t<id_type_t::ANIMATION>;
using animctrlid_t = id_t<id_type_t::ANIMATION_CONTROLLER>;

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

	entity_t playerEntity;
};

} // namespace te
