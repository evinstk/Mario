#include "reducer_world.hpp"
#include "entity_state.hpp"
#include <tegl/readtmx.hpp>

namespace te {

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

void loadEntity(entitystate_t& state, const decltype(tmx_t::objectgroups)& objectgroups, const mapstrdata_t& mapData) {
	loadTranslations(state.translations, objectgroups);
	loadTilesetSprites(state.tilesetSprites, objectgroups);
	loadVelocities(state.velocities);
	loadAnimators(state.animators, objectgroups, mapData.ctrlMap);
}

} // namespace te
