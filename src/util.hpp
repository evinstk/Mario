#pragma once
#include "types.hpp"

namespace te {

inline bool isValid(tileid_t tile) {
	return tile.id.first.id > 0;
}

} // namespace te
