#pragma once
#include "types.hpp"
#include <EASTL/vector.h>

namespace te {

struct soundeffectstate_t {
	eastl::vector<eastl::pair<musiccmd_t, musicid_t>> musicCommandQueue;
};

extern const soundeffectstate_t& gSoundEffect;

}
