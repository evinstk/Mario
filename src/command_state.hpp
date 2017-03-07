#pragma once
#include "types.hpp"
#include <EASTL/vector.h>

union SDL_Event;

namespace te {

struct gamestate_t;

struct commandstate_t {
	eastl::vector<playercmd_t> commandQueue;

	void process(const SDL_Event& evt);
	void flush();

	const gamestate_t& game;
	commandstate_t(const gamestate_t& g) : game(g) {}
};

}
