#pragma once

#include <tegl/types.hpp>
#include <memory>

namespace te {

class SpriteRenderer {
public:
	static SpriteRenderer *get();
private:
	ShaderProgram m_program;
	static std::unique_ptr<SpriteRenderer> m_instance;
};

} // namespace te
