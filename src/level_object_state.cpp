#include "level_object_state.hpp"

namespace te {

struct tmx_t;

static levelobjectstate_t gState;
const levelobjectstate_t& gLevelObject = gState;

void loadLevelObjects(levelobjectstate_t& state, const tmx_t& tmx, levelid_t levelID);
void loadLevelObjects(const tmx_t& tmx, levelid_t levelID) {
	loadLevelObjects(gState, tmx, levelID);
}

} // namespace te
