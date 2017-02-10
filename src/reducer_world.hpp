#pragma once
#include <SDL.h>

namespace te {

struct worldstate_t;
struct tmx_t;

void stepWorld(worldstate_t& state, float dt, const Uint8 *keyboardState);
void loadWorld(worldstate_t& state, const tmx_t& tmx);
void inputWorld(worldstate_t& state, const SDL_Event& evt);

} // namespace te
