#include "world_state.hpp"
#include "texture_manager.hpp"
#include <tegl/readtmx.hpp>
#include <EASTL/algorithm.h>
#include <EASTL/iterator.h>

namespace te {

static void stepTranslations(const entitymap_t<glm::vec3>& velocities,
					  float dt,
					  entitymap_t<glm::vec3>& translations) {
	for (const auto& velocityRow : velocities) {
		translations[velocityRow.first] += velocityRow.second * dt;
	}
}

void stepWorld(worldstate_t& state, float dt) {
	stepTranslations(state.velocities, dt, state.translations);
}

static void loadMap(map_t& map, const tmx_t& tmx) {
	map.size = glm::ivec2(tmx.width, tmx.height);
	map.tileSize = glm::ivec2(tmx.tilewidth, tmx.tileheight);
}

static void loadTilesets(vector_t<tileset_t>& tilesets, const tmx_t& tmx) {

	tilesets.clear();
	for (const auto& tmxtileset : tmx.tilesets) {
		GLuint textureID = gTextureManager->load(std::string("tiled/" + tmxtileset.images[0].source).c_str());
		tileset_t tileset = {
			.texture = textureID,
			.tileSize = glm::ivec2(tmxtileset.tilewidth, tmxtileset.tileheight),
			.spacing = tmxtileset.spacing,
			.margin = tmxtileset.margin,
			.columns = tmxtileset.columns,
			.firstgid = tmxtileset.firstgid,
			.tilecount = tmxtileset.tilecount
		};
		tilesets.push_back(tileset);
	}
}

static void loadLayers(vector_t<layer_t>& layers,
				const std::vector<tmxlayer_t>& tmxlayers) {
	layers.clear();
	eastl::transform(tmxlayers.begin(), tmxlayers.end(), eastl::back_inserter(layers), [](const tmxlayer_t& tmxlayer) {

			layer_t layer = {
				.gids = {},
				.layerIndex = tmxlayer.layerIndex,
				.size = glm::ivec2(tmxlayer.width, tmxlayer.height)
			};

			eastl::transform(tmxlayer.data.tiles.begin(),
							 tmxlayer.data.tiles.end(),
							 eastl::back_inserter(layer.gids),
							 [](tmxtile_t tile) {
								 return tile.gid;
							 });

			return layer;
		});
}

void loadWorld(worldstate_t& state, const tmx_t& tmx) {
	loadMap(state.map, tmx);
	loadTilesets(state.tilesets, tmx);
	loadLayers(state.layers, tmx.layers);
}

} // namespace te
