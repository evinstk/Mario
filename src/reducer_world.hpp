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

void stepWorld(worldstate_t& state, float dt, const Uint8 *keyboardState);
void stepEntity(entitystate_t& state, float dt, const Uint8 *keyboardState, entity_t playerEntity, const animctrlmap_t<animctrl_t>& controllers, const animmap_t<animation_t>& animations);
void loadGame(gamestate_t& state, const tsxtileset_t& tileset, const char *pathname);
void loadTileset(tilesetstate_t& state, const tsxtileset_t& tileset, const char *pathname);
void loadGame(gamestate_t& state, const tmx_t& tmx, const char *pathname);
void loadLevel(levelstate_t& state, const tmx_t& tmx, const char *pathname, const stringmap_t<tilesetid_t>& tilesetIDs);
void loadWorld(worldstate_t& state, const tmx_t& tmx);
void loadEntity(entitystate_t& state, const decltype(tmx_t::objectgroups)& tmx, const mapstrdata_t& mapData);
void inputWorld(worldstate_t& state, const SDL_Event& evt);

} // namespace te
