#pragma once
#include <tegl/types.hpp>

namespace te {

struct gamestate_t;

class TextRenderer {
public:
	TextRenderer();
	void draw(const gamestate_t& state);
private:
	Texture m_CharTexture;
};

} // namespace te
