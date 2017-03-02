#include "game_action.hpp"
#include "util.hpp"

namespace te {

void runMusicCommandQueue(levelid_t levelID);

void runGame(levelid_t levelID) {
	runWorld(levelID);
	runMusicCommandQueue(levelID);
}

} // namespace te
