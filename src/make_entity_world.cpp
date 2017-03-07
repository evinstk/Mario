#include "game_action.hpp"
#include "world_state.hpp"
#include "game_values.hpp"
#include "util.hpp"

namespace te {

void worldstate_t::makeEntity() {
	newEntityIDs.clear();
	for (auto it = newEntityQueue.begin(), end = newEntityQueue.end(); it < end; ++it) {
		entityid_t newID;
		auto freeIDIt = freeIDs.rbegin();
		if (freeIDIt != freeIDs.rend()) {
			newID = entityid_t(*freeIDIt);
			freeIDs.erase(freeIDIt);
		} else {
			newID = entityid_t(nextEntityID++);
		}
		newEntityIDs.push_back(newID);
	}
	te::makeEntity(entity, *pGame);
	newEntityQueue.clear();
	newEntityIDs.clear();
}

} // namespace te
