#include "tileset_state.hpp"

namespace te {

struct tsxtileset_t;

static tilesetstate_t gState;
const tilesetstate_t& gTileset = gState;

void loadTileset(tilesetstate_t& state, const tsxtileset_t& tileset, const char *pathname);
void loadTileset(const tsxtileset_t& tileset, const char *pathname) {
	loadTileset(gState, tileset, pathname);
}

} // namespace te
