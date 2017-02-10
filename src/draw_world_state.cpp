#include "world_state.hpp"

namespace te {

void DrawWorldState(const worldstate_t& state) {
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

} // namespace te
