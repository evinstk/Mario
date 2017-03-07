#pragma once
#include "types.hpp"

union SDL_Event;

namespace te {

struct gamestate_t;
struct tsxtileset_t;
struct tmx_t;

void inputGame(gamestate_t& state, const Uint8 *keyboardState);

void stepGame(gamestate_t& state, float dt);

void loadGame(gamestate_t& state, const tsxtileset_t& tileset, const char *pathname);

void loadSound(gamestate_t& state, chunkptr_t&& chunk, const char* pathname);

void loadMusic(gamestate_t& state, musicptr_t&& chunk, const char* pathname);

void loadGame(gamestate_t& state, const tmx_t& tmx, const char *pathname);

void runGame(gamestate_t& state, levelid_t levelID);

void processGame(gamestate_t& state, const SDL_Event& evt);

void dispatchGame(gamestate_t& state);

void makeEntity(gamestate_t& state);

void destroyEntity(gamestate_t& state);

} // namespace te
