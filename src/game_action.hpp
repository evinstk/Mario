#pragma once
#include "types.hpp"
#include <tegl/readtmx.hpp>
#include <SDL.h>
#include <SDL_mixer.h>
#include <memory>

namespace te {

struct gamestate_t;
struct tilesetstate_t;
struct soundstate_t;
struct levelstate_t;
struct levelobjectstate_t;

void inputGame(gamestate_t& state, const Uint8 *keyboardState);

void stepGame(gamestate_t& state, float dt);

void loadGame(gamestate_t& state, const tsxtileset_t& tileset, const char *pathname);
void loadTileset(tilesetstate_t& state, const tsxtileset_t& tileset, const char *pathname);

void loadSound(gamestate_t& state, chunkptr_t&& chunk, const char* pathname);
void loadSound(soundstate_t& state, chunkptr_t&& chunk, const char *pathname);

void loadMusic(gamestate_t& state, musicptr_t&& chunk, const char* pathname);
void loadMusic(soundstate_t& state, musicptr_t&& chunk, const char *pathname);

void loadGame(gamestate_t& state, const tmx_t& tmx, const char *pathname);
void loadLevel(levelstate_t& state, const tmx_t& tmx, const char *pathname, const tilesetstate_t& tilesetState, const gamestate_t& game);
void loadLevelObjects(levelobjectstate_t& state, const tmx_t& tmx, levelid_t levelID, const gamestate_t& game);

void runGame(gamestate_t& state, levelid_t levelID);

void processGame(gamestate_t& state, const SDL_Event& evt);

void dispatchGame(gamestate_t& state);

void makeEntity(gamestate_t& state);

void destroyEntity(gamestate_t& state);

} // namespace te
