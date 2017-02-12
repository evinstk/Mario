#pragma once
#include "types.hpp"
#include <tegl/readtmx.hpp>
#include <SDL.h>

namespace te {

struct worldstate_t;
struct entitystate_t;

void stepWorld(worldstate_t& state, float dt, const Uint8 *keyboardState);
void stepEntity(entitystate_t& state, float dt, const Uint8 *keyboardState, entity_t playerEntity, const animctrlmap_t<animctrl_t>& controllers, const animmap_t<animation_t>& animations);
void loadWorld(worldstate_t& state, const tmx_t& tmx);
void loadEntity(entitystate_t& state, const decltype(tmx_t::objectgroups)& tmx, const mapstrdata_t& mapData);
void inputWorld(worldstate_t& state, const SDL_Event& evt);

} // namespace te
