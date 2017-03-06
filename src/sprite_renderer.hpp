#pragma once

#include <tegl/types.hpp>

namespace te {

struct gamestate_t;

class SpriteRenderer {
public:
	SpriteRenderer();

	void draw(const gamestate_t& state);
private:
	ShaderProgram m_shader;

	GLuint m_projectionLoc;
	GLuint m_viewLoc;
	GLuint m_tileSizeLoc;
	GLuint m_spacingLoc;
	GLuint m_marginLoc;
	GLuint m_columnsLoc;

	VertexArray m_vertexArray;
	Buffer m_translationBuf;
	Buffer m_idBuf;

	int m_buffersCount;
};

} // namespace te
