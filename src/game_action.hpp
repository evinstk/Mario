#pragma once
#include "types.hpp"
#include <tegl/readtmx.hpp>
#include <SDL.h>

namespace te {

struct gamestate_t;
struct worldstate_t;
struct entitystate_t;
struct tilesetstate_t;
struct levelstate_t;
struct levelobjectstate_t;

void inputGame(gamestate_t& state, const Uint8 *keyboardState);
void inputWorld(worldstate_t& state, const Uint8 *keyboardState, const gamestate_t& game);
void inputEntity(entitystate_t& state, const Uint8 *keyboardState, const gamestate_t& game);
void stepGame(gamestate_t& state, float dt);
void stepWorld(worldstate_t& state, float dt, const gamestate_t& gameState);
void stepEntity(entitystate_t& state, float dt, const gamestate_t& gameState);
void loadGame(gamestate_t& state, const tsxtileset_t& tileset, const char *pathname);
void loadTileset(tilesetstate_t& state, const tsxtileset_t& tileset, const char *pathname);
void loadGame(gamestate_t& state, const tmx_t& tmx, const char *pathname);
void loadLevel(levelstate_t& state, const tmx_t& tmx, const char *pathname, const tilesetstate_t& tilesetState, const gamestate_t& game);
void loadLevelObjects(levelobjectstate_t& state, const tmx_t& tmx, levelid_t levelID, const gamestate_t& game);
void runGame(gamestate_t& state, levelid_t levelID);
void runWorld(worldstate_t& state, levelid_t levelID, const levelstate_t& levelState);
void runEntity(entitystate_t& state, levelid_t levelID, const levelobjectstate_t& objects);
void processGame(gamestate_t& state, const SDL_Event& evt);
void processWorld(worldstate_t& state, const SDL_Event& evt, const gamestate_t& game);

} // namespace te
