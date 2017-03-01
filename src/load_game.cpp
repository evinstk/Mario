#include "game_action.hpp"
#include "game_state.hpp"

namespace te {

void loadGame(gamestate_t& state, const tsxtileset_t& tileset, const char *pathname) {
	loadTileset(tileset, pathname);
}

void loadGame(gamestate_t& state, const tmx_t& tmx, const char *pathname) {
	loadLevel(state.level, tmx, pathname, state);
}

void loadSound(gamestate_t& state, chunkptr_t&& chunk, const char *pathname) {
	loadSound(eastl::move(chunk), pathname);
}

void loadMusic(gamestate_t& state, musicptr_t&& chunk, const char *pathname) {
	loadMusic(eastl::move(chunk), pathname);
}

} // namespace te
