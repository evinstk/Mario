#include "game_action.hpp"
#include "sound_state.hpp"

namespace te {

void loadSound(soundstate_t& state, std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>&& chunk, const char *pathname) {
	soundid_t soundID(state.nextSoundID++);
	state.soundID.insert({ eastl::string(pathname), soundID });
	decltype(soundstate_t::chunk)::value_type newRow(soundID, eastl::move(chunk));
	state.chunk.insert(eastl::move(newRow));
}

} // namespace te
