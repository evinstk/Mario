#include "world_state.hpp"
#include "action.hpp"
#include "texture_manager.hpp"
#include <tegl/readtmx.hpp>
#include <EASTL/algorithm.h>
#include <EASTL/iterator.h>

namespace te {

inline void reduceVelocities(const decltype(worldstate_t::velocities)& in,
							 const action_t& action,
							 decltype(worldstate_t::velocities)& out) {
	out.clear();
	eastl::copy(in.begin(), in.end(), eastl::back_inserter(out));
}

void reduceTranslations(const decltype(worldstate_t::velocities)& velocities,
						const decltype(worldstate_t::translations)& translations,
						float dt,
						decltype(worldstate_t::translations)& outTranslations) {
	outTranslations.clear();
	eastl::copy(translations.begin(), translations.end(), eastl::back_inserter(outTranslations));

	for (const auto& row : velocities) {
		const auto entityID = row.first;
		const auto velocity = row.second;
		const auto currTranslation = translations.find(entityID);
		outTranslations[entityID] = currTranslation->second * velocity;
	}
}

void reduceTranslations(const decltype(worldstate_t::velocities)& velocities,
						const decltype(worldstate_t::translations)& translations,
						const action_t& action,
						decltype(worldstate_t::translations)& outTranslations) {
	switch (action.type) {
	case actiontype_t::STEP:
		reduceTranslations(velocities, translations, action.payload.dt, outTranslations);
		return;
	default:
		outTranslations.clear();
		eastl::copy(translations.begin(), translations.end(), eastl::back_inserter(outTranslations));
		return;
	}
}

void reduceTilesets(const decltype(worldstate_t::tilesets)& in,
					const tmx_t& tmx,
					decltype(worldstate_t::tilesets)& out) {

	out.clear();
	for (const auto& tmxtileset : tmx.tilesets) {
		GLuint textureID = gTextureManager->load(std::string("tiled/" + tmxtileset.images[0].source).c_str());
		tileset_t tileset = {
			.texture = textureID
		};
		out.push_back(tileset);
	}
}

void reduceTilesets(const decltype(worldstate_t::tilesets)& in,
					const action_t& action,
					decltype(worldstate_t::tilesets)& out) {
	switch (action.type) {
	case actiontype_t::LOAD_LEVEL:
		reduceTilesets(in, *action.payload.tmx, out);
		return;
	default:
		out.clear();
		eastl::copy(in.begin(), in.end(), eastl::back_inserter(out));
		return;
	}
}

void reduceLayers(const decltype(worldstate_t::layers)& in,
				  const std::vector<tmxlayer_t>& layers,
				  decltype(worldstate_t::layers)& out) {
	out.clear();
	eastl::transform(layers.begin(), layers.end(), eastl::back_inserter(out), [](const tmxlayer_t& tmxlayer) {

			layer_t layer = {
				.layerIndex = tmxlayer.layerIndex
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

void reduceLayers(const decltype(worldstate_t::layers)& in,
				  const action_t& action,
				  decltype(worldstate_t::layers)& out) {
	switch (action.type) {
	case actiontype_t::LOAD_LEVEL:
		reduceLayers(in, action.payload.tmx->layers, out);
		return;
	default:
		out.clear();
		eastl::copy(in.begin(), in.end(), eastl::back_inserter(out));
		return;
	}
}

void reduceWorld(const worldstate_t& in,
				 const action_t& action,
				 worldstate_t& out) {

	reduceVelocities(in.velocities, action, out.velocities);
	reduceTranslations(in.velocities, in.translations, action, out.translations);
	reduceTilesets(in.tilesets, action, out.tilesets);
	reduceLayers(in.layers, action, out.layers);
}

}
