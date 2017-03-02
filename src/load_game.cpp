#include "game_action.hpp"

namespace te {

void loadGame(const tsxtileset_t& tileset, const char *pathname) {
	loadTileset(tileset, pathname);
}

void loadGame(const tmx_t& tmx, const char *pathname) {
	loadLevel(tmx, pathname);
}

} // namespace te
