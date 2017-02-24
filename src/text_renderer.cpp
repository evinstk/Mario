#include "text_renderer.hpp"
#include "game_values.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <EASTL/vector.h>
#include <cassert>

namespace te {

constexpr int numChars = 128;

TextRenderer::TextRenderer()
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

	const int cellSize = maxWidth * maxHeight;
	const int size = cellSize * numChars;
	eastl::vector<GLubyte> pixels(size, 0);
	for (GLubyte ch = 0; ch < numChars; ++ch) {

		FT_Error err = FT_Load_Char(face, ch, FT_LOAD_RENDER);
		assert(err == 0);

		const unsigned char *pBuffer = face->glyph->bitmap.buffer;

		for (int row = 0, rowNum = face->glyph->bitmap.rows; row < rowNum; ++row) {
			for (int col = 0, colNum = face->glyph->bitmap.width; col < colNum; ++col) {

				GLubyte pixel = pBuffer[row * colNum + col];
				pixels[cellSize * ch + row * maxWidth + col] = pixel;
			}
		}
	}

	glBindTexture(GL_TEXTURE_2D, m_CharTexture);
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
}

} // namespace te
