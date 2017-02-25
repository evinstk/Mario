#pragma once
#include <tegl/types.hpp>
#include <glm/glm.hpp>
#include <EASTL/array.h>
#include <EASTL/string.h>

namespace te {

struct gamestate_t;

class TextRenderer {
public:
	TextRenderer();
	void draw(const eastl::string& text,
			  const glm::ivec2& location,
			  const glm::mat4& projection);
private:
	static constexpr int numChars = 128;

	eastl::array<glm::ivec2, numChars> m_bearings;
	eastl::array<int, numChars> m_advances;
	glm::ivec2 m_cellSize;
	Texture m_charTexture;

	VertexArray m_vertexArray;

	Buffer m_translationBuf;
	Buffer m_textBuf;
	int m_bufSize;

	ShaderProgram m_shader;

	GLuint m_projectionLoc;
	GLuint m_cellSizeLoc;
};

} // namespace te
