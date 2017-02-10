#pragma once

namespace te {

struct tmx_t;
struct action_t;

void reduceWorld(const worldstate_t& in, const action_t& action, worldstate_t& out);
//void reduceWorld(const worldstate_t& in, const tmx_t& tmx, worldstate_t& out);

} // namespace te
