#pragma once
#include "tileset_state.hpp"
#include "sound_state.hpp"
#include "level_state.hpp"
#include "command_state.hpp"
#include "world_state.hpp"
#include "sound_effect_state.hpp"

namespace te {

struct gamestate_t {
	tilesetstate_t tileset;
	soundstate_t sound;
	levelstate_t level;
	commandstate_t command;
	worldstate_t world;

	soundeffectstate_t soundEffect;

	gamestate_t()
		: command(*this)
		, soundEffect(*this)
	{}
};

} // namespace te
