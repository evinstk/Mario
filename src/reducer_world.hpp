#pragma once

namespace te {

struct tmx_t;
struct action_t;

void stepWorld(worldstate_t& state, float dt);
void loadWorld(worldstate_t& state, const tmx_t& tmx);

} // namespace te
