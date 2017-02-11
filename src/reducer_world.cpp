#include "world_state.hpp"
#include "texture_manager.hpp"
#include <tegl/readtmx.hpp>
#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include <EASTL/algorithm.h>
#include <EASTL/iterator.h>
#include <glm/gtx/transform.hpp>
#include <algorithm>
#include <numeric>
#include <SDL.h>

namespace te {

static void stepVelocities(entitymap_t<glm::vec3>& velocities, entity_t playerEntity, const Uint8 *keyboard) {
	static constexpr float SPEED = 32.0f;
	velocities[playerEntity].x = (keyboard[SDL_SCANCODE_D] - keyboard[SDL_SCANCODE_A]) * SPEED;
}

static void stepTranslations(const entitymap_t<glm::vec3>& velocities,
					  float dt,
					  entitymap_t<glm::vec3>& translations) {
	for (const auto& velocityRow : velocities) {
		translations[velocityRow.first] += velocityRow.second * dt;
	}
}

static void stepAnimators(const animctrlmap_t<animctrl_t>& controllers,
						  const entitymap_t<glm::vec3>& velocities,
						  float dt,
						  entitymap_t<animator_t>& animators) {

	for (auto& animatorRow : animators) {
		animator_t& animator = animatorRow.second;
		animator.elapsed += dt;
		entity_t entityID = animatorRow.first;
		glm::vec3 velocity = velocities.find(entityID)->second;
		const animctrl_t& controller = controllers.find(animator.controller)->second;
		animid_t newAnim;
		if (velocity.x > 0) {
			newAnim = controller.walkRight;
		} else if (velocity.x < 0) {
			newAnim = controller.walkLeft;
		}
		if (newAnim != animator.animation && newAnim.id > 0) {
			animator.animation = newAnim;
			animator.elapsed = 0;
		}
	}
}

static void stepSprites(const entitymap_t<animator_t>& animators,
						const animmap_t<animation_t>& animations,
						entitymap_t<int>& sprites) {
	for (const auto& animatorRow : animators) {
		const auto& animator = animatorRow.second;
		if (animator.animation.id == 0) {
			continue;
		}
		const auto& animation = animations.find(animator.animation)->second;
		int elapsedMS = animator.elapsed * 1000;
		int clampedElapsed = elapsedMS % animation.totalDuration;
		int acc = 0;
		for (const auto& frame : animation.frames) {
			acc += frame.duration;
			if (clampedElapsed < acc) {
				entity_t entityID = animatorRow.first;
				sprites[entityID] = frame.gid;
				break;
			}
		}
	}
}

static void stepView(const glm::vec3& playerTranslation,
					 glm::mat4& view) {
	view = glm::translate(glm::vec3(-playerTranslation.x,
									-playerTranslation.y,
									0));
}

void stepWorld(worldstate_t& state, float dt, const Uint8 *keyboardState) {
	stepVelocities(state.velocities, state.playerEntity, keyboardState);
	stepTranslations(state.velocities, dt, state.translations);
	stepAnimators(state.animationControllers, state.velocities, dt, state.animators);
	stepSprites(state.animators, state.animations, state.tilesetSprites);
	stepView(state.translations[state.playerEntity], state.view);
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

static void loadTranslations(entitymap_t<glm::vec3>& translations,
							 const decltype(tmx_t::objectgroups)& objectgroups) {
	translations.clear();
	for (const auto& group : objectgroups) {
		for (const auto& object : group.objects) {
			entity_t entityID(object.id);
			glm::ivec3 translation(object.x, object.y - object.height, group.layerIndex);
			translations.insert({ entityID, translation });
		}
	}
}

static void loadTilesetSprites(entitymap_t<int>& sprites,
							   const decltype(tmx_t::objectgroups)& objectgroups) {
	sprites.clear();
	for (const auto& group : objectgroups) {
		for (const auto& object : group.objects) {
			if (object.gid > 0) {
				sprites.insert({ entity_t(object.id), object.gid });
			}
		}
	}
}

static inline void loadVelocities(entitymap_t<glm::vec3> velocities) {
	velocities.clear();
}

static void loadAnimations(animmap_t<animation_t>& animations,
						   const decltype(tmx_t::tilesets)& tilesets) {
	animations.clear();
	for (const auto& tileset : tilesets) {
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
						.gid = tileset.firstgid + tmxframe.tileid,
						.duration = tmxframe.duration
					};
					animation.frames.push_back(frame);
				}
				animid_t animID(tileset.firstgid + tile.id);
				animations.insert({ animID, eastl::move(animation) });
			}
		}
	}
}

static void loadAnimationControllers(animctrlmap_t<animctrl_t>& controllers,
									 const decltype(tmx_t::tilesets)& tilesets,
									 eastl::hash_map<eastl::string, animctrlid_t>& ctrlIDMap) {
	eastl::hash_map<eastl::string, animctrl_t> ctrlMap;

	for (const auto& tileset : tilesets) {
		for (const auto& tile : tileset.tiles) {
			auto ctrlPropIt = tile.properties.find("animctrl");
			if (ctrlPropIt != tile.properties.end()) {
				auto ctrlStr = ctrlPropIt->second;
				auto motionIt = tile.properties.find("motion");
				assert(motionIt != tile.properties.end());
				auto& ctrl = ctrlMap[ctrlStr.c_str()];
				animid_t animID(tileset.firstgid + tile.id);
				auto motionStr = motionIt->second;
				if (motionStr == "walk-right") {
					ctrl.walkRight = animID;
				} else if (motionStr == "walk-left") {
					ctrl.walkLeft = animID;
				}
			}
		}
	}

	controllers.clear();
	int nextCtrlID = 1;
	for (const auto& ctrlRow : ctrlMap) {
		animctrlid_t ctrlID(nextCtrlID++);
		controllers.insert({ ctrlID, ctrlRow.second });
		ctrlIDMap[ctrlRow.first] = ctrlID;
	}
}

static void loadAnimators(entitymap_t<animator_t>& animators,
						  const decltype(tmx_t::objectgroups)& objectgroups,
						  const eastl::hash_map<eastl::string, animctrlid_t>& ctrlMap) {
	animators.clear();
	for (const auto& group : objectgroups) {
		for (const auto& object : group.objects) {
			auto ctrlPropIt = object.properties.find("animctrl");
			if (ctrlPropIt != object.properties.end()) {
				auto ctrlIDIt = ctrlMap.find_as(ctrlPropIt->second.c_str());
				assert(ctrlIDIt != ctrlMap.end());
				animctrlid_t ctrlID = ctrlIDIt->second;
				animator_t animator = {
					.controller = ctrlID,
					.animation = {},
					.elapsed = 0.0f
				};
				entity_t entityID(object.id);
				animators.insert({ entityID, animator });
			}
		}
	}
}

void loadWorld(worldstate_t& state, const tmx_t& tmx) {
	loadMap(state.map, tmx);
	loadTilesets(state.tilesets, tmx);
	loadLayers(state.layers, tmx.layers);

	loadAnimations(state.animations, tmx.tilesets);
	eastl::hash_map<eastl::string, animctrlid_t> ctrlMap;
	loadAnimationControllers(state.animationControllers, tmx.tilesets, ctrlMap);

	loadTranslations(state.translations, tmx.objectgroups);
	loadTilesetSprites(state.tilesetSprites, tmx.objectgroups);
	loadVelocities(state.velocities);
	loadAnimators(state.animators, tmx.objectgroups, ctrlMap);

	state.playerEntity = entity_t(0);
	for (const auto& group : tmx.objectgroups) {
		auto playerIt = std::find_if(group.objects.begin(), group.objects.end(), [](const auto& object) {
				return object.name == "player";
			});
		if (playerIt != group.objects.end()) {
			state.playerEntity = entity_t(playerIt->id);
			break;
		}
	}
	assert(state.playerEntity != entity_t(0));

	stepView(state.translations[state.playerEntity], state.view);
}

} // namespace te
