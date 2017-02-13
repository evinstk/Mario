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

void stepGame(gamestate_t& state, float dt, const Uint8 *keyboardState);
void stepWorld(worldstate_t& state, float dt, const Uint8 *keyboardState, const tilesetstate_t& tilesetState, const levelstate_t& levelState);
void stepEntity(entitystate_t& state, float dt, const Uint8 *keyboardState, entity_t playerEntity, levelid_t levelID, const tilesetstate_t& tilesetState, const levelstate_t& levelState);
void loadGame(gamestate_t& state, const tsxtileset_t& tileset, const char *pathname);
void loadTileset(tilesetstate_t& state, const tsxtileset_t& tileset, const char *pathname);
void loadGame(gamestate_t& state, const tmx_t& tmx, const char *pathname);
void loadLevel(levelstate_t& state, const tmx_t& tmx, const char *pathname, const stringmap_t<tilesetid_t>& tilesetIDs);
//void loadWorld(worldstate_t& state, const tmx_t& tmx);
//void loadEntity(entitystate_t& state, const decltype(tmx_t::objectgroups)& tmx, const mapstrdata_t& mapData);
void runGame(gamestate_t& state, levelid_t levelID);
void runWorld(worldstate_t& world, levelid_t levelID, const levelstate_t& levelState);
void inputWorld(worldstate_t& state, const SDL_Event& evt);

} // namespace te
