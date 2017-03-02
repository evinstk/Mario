#pragma once
#include "types.hpp"
#include <tegl/readtmx.hpp>
#include <SDL.h>
#include <SDL_mixer.h>
#include <memory>

namespace te {

struct worldstate_t;
struct entitystate_t;
struct tilesetstate_t;
struct soundstate_t;
struct levelstate_t;
struct levelobjectstate_t;

void inputGame(const Uint8 *keyboardState);
void inputWorld(const Uint8 *keyboardState);
void inputEntity(const Uint8 *keyboardState);

void stepGame(float dt);
void stepWorld(float dt);
void stepEntity(float dt);

void loadGame(const tsxtileset_t& tileset, const char *pathname);
void loadTileset(const tsxtileset_t& tileset, const char *pathname);

void loadSound(chunkptr_t&& chunk, const char *pathname);

void loadMusic(musicptr_t&& chunk, const char *pathname);

void loadGame(const tmx_t& tmx, const char *pathname);
void loadLevel(const tmx_t& tmx, const char *pathname);
void loadLevelObjects(const tmx_t& tmx, levelid_t levelID);

void runGame(levelid_t levelID);
void runWorld(levelid_t levelID);
void runEntity(levelid_t levelID);

void processGame(const SDL_Event& evt);
void processWorld(const SDL_Event& evt);
void processEntity(const SDL_Event& evt, entityid_t playerEntity, bool& jump);

void makeEntity();
void makeEntityWorld();
void makeEntityEntity();

void destroyEntity();
void destroyEntityWorld();
void destroyEntityEntity();

void flushSoundQueue();
void flushMusicCommandQueue();

} // namespace te
