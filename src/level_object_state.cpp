#include "level_object_state.hpp"

namespace te {

struct tmx_t;
struct gamestate_t;

static levelobjectstate_t gState;
const levelobjectstate_t& gLevelObject = gState;

void loadLevelObjects(levelobjectstate_t& state, const tmx_t& tmx, levelid_t levelID, const gamestate_t& game);
void loadLevelObjects(const tmx_t& tmx, levelid_t levelID, const gamestate_t& game) {
	loadLevelObjects(gState, tmx, levelID, game);
}

} // namespace te
