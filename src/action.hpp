#pragma once

namespace te {

struct tmx_t;

enum class actiontype_t {
	STEP,
	LOAD_LEVEL
};

struct action_t {
	actiontype_t type;
	union {
		float dt;
		const tmx_t* tmx;
	} payload;
};

} // namespace te
