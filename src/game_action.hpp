#pragma once
#include "types.hpp"
#include <tegl/readtmx.hpp>
#include <SDL.h>
#include <SDL_mixer.h>
#include <memory>

namespace te {

struct gamestate_t;
struct worldstate_t;
struct entitystate_t;
struct tilesetstate_t;
struct soundstate_t;
struct levelstate_t;
struct levelobjectstate_t;

void inputGame(gamestate_t& state, const Uint8 *keyboardState);
void inputWorld(const Uint8 *keyboardState, const gamestate_t& game);
void inputEntity(const Uint8 *keyboardState, const gamestate_t& game);

void stepGame(gamestate_t& state, float dt);
void stepWorld(float dt, const gamestate_t& gameState);
void stepEntity(float dt, const gamestate_t& gameState);

void loadGame(gamestate_t& state, const tsxtileset_t& tileset, const char *pathname);
void loadTileset(const tsxtileset_t& tileset, const char *pathname);

void loadSound(gamestate_t& state, chunkptr_t&& chunk, const char* pathname);
void loadSound(chunkptr_t&& chunk, const char *pathname);

void loadMusic(gamestate_t& state, musicptr_t&& chunk, const char* pathname);
void loadMusic(musicptr_t&& chunk, const char *pathname);

void loadGame(gamestate_t& state, const tmx_t& tmx, const char *pathname);
void loadLevel(levelstate_t& state, const tmx_t& tmx, const char *pathname, const gamestate_t& game);
void loadLevelObjects(const tmx_t& tmx, levelid_t levelID, const gamestate_t& game);

void runGame(gamestate_t& state, levelid_t levelID);
void runWorld(levelid_t levelID, const levelstate_t& levelState);
void runEntity(levelid_t levelID);

void processGame(gamestate_t& state, const SDL_Event& evt);
void processWorld(const SDL_Event& evt, const gamestate_t& game);
void processEntity(const SDL_Event& evt, entityid_t playerEntity, bool& jump);

void makeEntity(gamestate_t& state);
void makeEntityWorld(const gamestate_t& game);
void makeEntity(const gamestate_t& game);

void destroyEntity(gamestate_t& state);
void destroyEntityWorld(const gamestate_t& game);
void destroyEntity();

void flushSoundQueue();
void flushMusicCommandQueue();

} // namespace te
