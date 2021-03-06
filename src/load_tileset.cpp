#include "game_action.hpp"
#include "tileset_state.hpp"
#include "texture_manager.hpp"
#include <tegl/readtmx.hpp>
#include <numeric>

namespace te {

static bool loadSource(stringmap_t<tilesetid_t>& source, int& nextID, const char *pathname) {
	auto existingIt = source.find_as(pathname);
	if (existingIt != source.end()) {
		return false;
	}

	tilesetid_t id(nextID++);
	source.insert({ eastl::string(pathname), id });
	return true;
}

static void loadTileset(tilesetmap_t<tileset_t>& state, const tsxtileset_t& tsxtileset, tilesetid_t id) {
	// TODO: remove side effect
	GLuint textureID = gTextureManager->load(std::string("tilesets/" + tsxtileset.images[0].source).c_str());
	tileset_t tileset = {
		.texture = textureID,
		.tileSize = glm::ivec2(tsxtileset.tilewidth, tsxtileset.tileheight),
		.spacing = tsxtileset.spacing,
		.margin = tsxtileset.margin,
		.columns = tsxtileset.columns,
		.tilecount = tsxtileset.tilecount
	};
	state.insert({ id, tileset });
}

static void loadAnimation(animmap_t<animation_t>& state, const tsxtileset_t& tileset, tilesetid_t tilesetID) {
	for (const auto& tile : tileset.tiles) {
		if (tile.animation.frames.size() > 0) {
			int totalDuration = std::accumulate(tile.animation.frames.begin(), tile.animation.frames.end(), 0, [](int total, auto frame) {
					return total + frame.duration;
				});
			animation_t animation = {
				.totalDuration = totalDuration,
				.frames = {}
			};
			for (const auto& tmxframe : tile.animation.frames) {
				frame_t frame = {
					.tileid = tileid_t({ tilesetID, tmxframe.tileid }),
					.duration = tmxframe.duration
				};
				animation.frames.push_back(frame);
			}
			animid_t animID({ tilesetID, tile.id });
			state.insert({ animID, eastl::move(animation) });
		}
	}
}

static void loadAnimationID(stringmap_t<animid_t>& state, const tsxtileset_t& tileset, tilesetid_t tilesetID) {
	for (const auto& tile : tileset.tiles) {
		auto animationPropIt = tile.properties.find("animation");
		if (animationPropIt != tile.properties.end()) {
			animid_t animID({ tilesetID, tile.id });
			state.insert({ eastl::string(animationPropIt->second.c_str()), animID });
		}
	}
}

static void loadCollider(stringmap_t<colliderid_t>& ids,
						 int& nextID,
						 collidermap_t<aabb_t>& colliders,
						 const tsxtileset_t& tileset) {
	for (const auto& tile : tileset.tiles) {
		auto colliderPropIt = tile.properties.find("collider");
		if (colliderPropIt != tile.properties.end()) {
			assert(tile.geometry.size() == 1);
			aabb_t boxCollider = {
				.pos = { tile.geometry[0].x, tile.geometry[0].y },
				.size = { tile.geometry[0].width, tile.geometry[0].height }
			};
			colliderid_t id(nextID++);
			ids.insert({ eastl::string(colliderPropIt->second.c_str()), id });
			colliders.insert({ id, boxCollider });
		}
	}
}

static void loadSolid(eastl::vector_set<tileid_t>& state,
					  const tsxtileset_t& tileset,
					  tilesetid_t tilesetID) {

	for (const auto& tile : tileset.tiles) {
		auto solidPropIt = tile.bProperties.find("solid");
		if (solidPropIt != tile.bProperties.end() && solidPropIt->second) {
			state.insert(tileid_t({ tilesetID, tile.id }));
		}
	}
}

static void loadTileID(stringmap_t<tileid_t>& state,
					   const tsxtileset_t& tileset,
					   tilesetid_t tilesetID) {

	for (const auto& tile : tileset.tiles) {
		auto spriteIter = tile.properties.find("name");
		if (spriteIter != tile.properties.end()) {
			state.insert({ eastl::string(spriteIter->second.c_str()), tileid_t({ tilesetID, tile.id }) });
		}
	}
}

void tilesetstate_t::loadTileset(const tsxtileset_t& tileset, const char *pathname) {
	bool loaded = loadSource(source, nextID, pathname);
	if (!loaded) {
		// has been loaded before: no more work here
		return;
	}

	tilesetid_t id = source.find_as(pathname)->second;
	te::loadTileset(this->tileset, tileset, id);
	loadAnimation(animation, tileset, id);
	loadAnimationID(animationID, tileset, id);
	loadCollider(colliderID, nextColliderID, collider, tileset);
	loadSolid(solid, tileset, id);
	loadTileID(tileID, tileset, id);
}

} // namespace te
