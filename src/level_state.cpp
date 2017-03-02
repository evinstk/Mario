#include "level_state.hpp"

namespace te {

struct tmx_t;

static levelstate_t gState;
const levelstate_t& gLevel = gState;

void loadLevel(levelstate_t& state, const tmx_t& tmx, const char *pathname);
void loadLevel(const tmx_t& tmx, const char *pathname) {
	loadLevel(gState, tmx, pathname);
}

} // namespace te
