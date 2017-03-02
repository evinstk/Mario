#include "level_state.hpp"

namespace te {

struct gamestate_t;
struct tmx_t;

static levelstate_t gState;
const levelstate_t& gLevel = gState;

void loadLevel(levelstate_t& state, const tmx_t& tmx, const char *pathname, const gamestate_t& game);
void loadLevel(const tmx_t& tmx, const char *pathname, const gamestate_t& game) {
	loadLevel(gState, tmx, pathname, game);
}

} // namespace te
