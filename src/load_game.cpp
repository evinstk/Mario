#include "game_action.hpp"
#include "game_state.hpp"

namespace te {

void loadGame(gamestate_t& state, const tsxtileset_t& tileset, const char *pathname) {
	loadTileset(state.tileset, tileset, pathname);
}

void loadGame(gamestate_t& state, const tmx_t& tmx, const char *pathname) {
	loadLevel(state.level, tmx, pathname, state.tileset, state);
}

void loadSound(gamestate_t& state, std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>&& chunk, const char *pathname) {
	loadSound(state.sound, std::move(chunk), pathname);
}

} // namespace te
