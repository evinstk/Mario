#include "text_renderer.hpp"
#include "game_values.hpp"
#include <tegl/util.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <EASTL/vector.h>
#include <EASTL/algorithm.h>
#include <EASTL/iterator.h>
#include <cassert>

namespace te {

TextRenderer::TextRenderer()
	: m_bufSize(0)
{
	FT_Library ft;
	FT_Error err = FT_Init_FreeType(&ft);
	assert(err == 0);

	FT_Face face;
	err = FT_New_Face(ft, FONT_NES, 0, &face);
	assert(err == 0);

	FT_Set_Pixel_Sizes(face, 0, FONT_PIXEL_SIZE);

	int maxWidth = 0, maxHeight = 0;
	for (GLubyte c = 0; c < numChars; ++c) {
		FT_Error err = FT_Load_Char(face, c, FT_LOAD_RENDER);
		assert(err == 0);

		int width = face->glyph->bitmap.width;
		if (width > maxWidth) {
			maxWidth = width;
		}

		int height = face->glyph->bitmap.rows;
		if (height > maxHeight) {
			maxHeight = height;
		}
	}

	m_cellSize.x = maxWidth;
	m_cellSize.y = maxHeight;

	int cellSize = maxWidth * maxHeight;
	const int size = cellSize * numChars;
	eastl::vector<GLubyte> pixels(size, 0);
	for (GLubyte ch = 0; ch < numChars; ++ch) {

		FT_Error err = FT_Load_Char(face, ch, FT_LOAD_RENDER);
		assert(err == 0);

		m_bearings[ch].x = face->glyph->bitmap_left;
		m_bearings[ch].y = face->glyph->bitmap_top;
		m_advances[ch]   = face->glyph->advance.x >> 6;

		const unsigned char *pBuffer = face->glyph->bitmap.buffer;

		for (int row = 0, rowNum = face->glyph->bitmap.rows; row < rowNum; ++row) {
			for (int col = 0, colNum = face->glyph->bitmap.width; col < colNum; ++col) {

				GLubyte pixel = pBuffer[row * colNum + col];
				pixels[cellSize * ch + row * maxWidth + col] = pixel;
			}
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, m_charTexture);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RED,
				 maxWidth,
				 maxHeight * numChars,
				 0,
				 GL_RED,
				 GL_UNSIGNED_BYTE,
				 pixels.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glBindVertexArray( m_vertexArray );

	glBindBuffer(GL_ARRAY_BUFFER, m_translationBuf);
	glVertexAttribIPointer(0, 2, GL_INT, 0, 0);
	glVertexAttribDivisor(0, 1);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_textBuf);
	glVertexAttribIPointer(1, 1, GL_INT, 0, 0);
	glVertexAttribDivisor(1, 1);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Shader vertexShader( GL_VERTEX_SHADER );
	auto vertexShaderSrc = LoadFile( "shaders/text.glvs" );
	CompileShader( vertexShader, vertexShaderSrc.c_str() );

	Shader fragmentShader( GL_FRAGMENT_SHADER );
	auto fragmentShaderSrc = LoadFile( "shaders/text.glfs" );
	CompileShader( fragmentShader, fragmentShaderSrc.c_str() );

	glAttachShader( m_shader, vertexShader );
	glAttachShader( m_shader, fragmentShader );
	glLinkProgram( m_shader );

	GLint programSuccess = GL_TRUE;
	glGetProgramiv( m_shader, GL_LINK_STATUS, &programSuccess );
	assert( programSuccess == GL_TRUE );

	m_projectionLoc = glGetUniformLocation(m_shader, "projection");
	m_cellSizeLoc   = glGetUniformLocation(m_shader, "cellSize");

	assert(glGetError() == GL_NO_ERROR);
}

void TextRenderer::draw(const eastl::string& text,
						const glm::ivec2& location,
						const glm::mat4& projection) {
	glUseProgram(m_shader);

	glUniformMatrix4fv(m_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform2iv(m_cellSizeLoc, 1, glm::value_ptr(m_cellSize));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_charTexture);

	using translation_t = glm::ivec2;
	using text_t = int;

	auto textSize = text.size();
	if (textSize > m_bufSize) {
		glBindBuffer(GL_ARRAY_BUFFER, m_translationBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(translation_t) * textSize, NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, m_textBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(text_t) * textSize, NULL, GL_DYNAMIC_DRAW);
		m_bufSize = textSize;
	}

	eastl::vector<text_t> intText;
	intText.reserve(textSize);
	eastl::transform(text.begin(), text.end(), eastl::back_inserter(intText), [](auto ch) {
			return (text_t)ch;
		});
	glBindBuffer(GL_ARRAY_BUFFER, m_textBuf);
	glBufferSubData(GL_ARRAY_BUFFER,
					0,
					sizeof(text_t) * textSize,
					intText.data());

	eastl::vector<translation_t> translations;
	translations.reserve(textSize);
	int baseX = location.x;
	eastl::transform(text.begin(), text.end(), eastl::back_inserter(translations), [baseX, this, &location] (auto ch) mutable {
			glm::ivec2 translation(baseX + m_bearings[ch].x, location.y - m_bearings[ch].y);
			baseX += m_advances[ch];
			return translation;
		});
	glBindBuffer(GL_ARRAY_BUFFER, m_translationBuf);
	glBufferSubData(GL_ARRAY_BUFFER,
					0,
					sizeof(translation_t) * textSize,
					translations.data());

	glBindVertexArray(m_vertexArray);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, textSize);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	assert(glGetError() == GL_NO_ERROR);
}

} // namespace te
