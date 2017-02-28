#include "sprite_renderer.hpp"
#include "game_state.hpp"
#include "util.hpp"
#include <tegl/types.hpp>
#include <tegl/util.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>

namespace te {

SpriteRenderer::SpriteRenderer()
	: m_buffersCount(0)
{
	Shader vertexShader( GL_VERTEX_SHADER );
	auto vertexShaderSrc = LoadFile( "shaders/tileset.glvs" );
	CompileShader( vertexShader, vertexShaderSrc.c_str() );

	Shader fragmentShader( GL_FRAGMENT_SHADER );
	auto fragmentShaderSrc = LoadFile( "shaders/tileset.glfs" );
	CompileShader( fragmentShader, fragmentShaderSrc.c_str() );

	glAttachShader( m_shader, vertexShader );
	glAttachShader( m_shader, fragmentShader );
	glLinkProgram( m_shader );

	GLint programSuccess = GL_TRUE;
	glGetProgramiv( m_shader, GL_LINK_STATUS, &programSuccess );
	assert( programSuccess == GL_TRUE );

	m_projectionLoc = glGetUniformLocation( m_shader, "projection" );
	m_viewLoc       = glGetUniformLocation( m_shader, "view" );
	m_tileSizeLoc   = glGetUniformLocation( m_shader, "tileSize" );
	m_spacingLoc    = glGetUniformLocation( m_shader, "spacing" );
	m_marginLoc     = glGetUniformLocation( m_shader, "margin" );
	m_columnsLoc    = glGetUniformLocation( m_shader, "columns" );

	glBindVertexArray( m_vertexArray );

	glBindBuffer( GL_ARRAY_BUFFER, m_translationBuf );
	glVertexAttribIPointer( 0, 3, GL_INT, 0, 0 );
	glVertexAttribDivisor( 0, 1 );
	glEnableVertexAttribArray( 0 );

	glBindBuffer( GL_ARRAY_BUFFER, m_idBuf );
	glVertexAttribIPointer( 1, 1, GL_INT, 0, 0 );
	glVertexAttribDivisor( 1, 1 );
	glEnableVertexAttribArray( 1 );

	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	assert( glGetError() == GL_NO_ERROR );
}

void SpriteRenderer::draw(const gamestate_t& state) {
	glUseProgram(m_shader);

	glUniformMatrix4fv(m_projectionLoc, 1, GL_FALSE, glm::value_ptr(state.world.projection));
	glm::mat4 view = state.world.view;
	glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray( m_vertexArray );

	auto mapIt = state.level.map.find(state.world.level);
	assert(mapIt != state.level.map.end());
	const auto& map = mapIt->second;

	eastl::vector<glm::ivec3> translations;
	eastl::vector<GLint> ids;
	eastl::vector<tileid_t> markedTiles;

	// iterate tilesets in outer loop to minimize texture bindings
	auto tilesetsIt = state.level.tilesets.find(state.world.level);
	assert(tilesetsIt != state.level.tilesets.end());
	const auto& tilesets = tilesetsIt->second;
	for (const auto& layerTileset : tilesets) {
		auto tilesetIt = state.tileset.tileset.find(layerTileset.tileset);
		assert(tilesetIt != state.tileset.tileset.end());
		const auto& tileset = tilesetIt->second;
		glBindTexture(GL_TEXTURE_2D, tileset.texture);

		glUniform2iv( m_tileSizeLoc, 1, glm::value_ptr( tileset.tileSize ) );
		glUniform1i( m_spacingLoc, tileset.spacing );
		glUniform1i( m_marginLoc, tileset.margin );
		glUniform1i( m_columnsLoc, tileset.columns );

		for (const auto& layer : state.world.layers) {

			for (int i = 0, layerSize = layer.tiles.size(); i < layerSize; ++i) {

				tileid_t tile = layer.tiles[i];
				tilesetid_t tileset = tile.id.first;
				GLint localID = tile.id.second;

				// if unmarked tile is in currently bound tileset,
				// find all tiles in layer, draw, and mark tile
				if (tileset == layerTileset.tileset
					&& eastl::find(markedTiles.begin(), markedTiles.end(), tile) == markedTiles.end()) {

					for (int j = i; j < layerSize; ++j) {
						if (layer.tiles[j] == tile) {

							translations.push_back(glm::ivec3( ( j % layer.size.x ) * map.tileSize.x,
															   ( j / layer.size.x ) * map.tileSize.y,
															   layer.layerIndex ));
							ids.push_back(localID);
						}
					}

					markedTiles.push_back(tile);
				}
			}

			markedTiles.clear();
		}

		for (const auto& spriteRow : gEntity.tilesetSprites) {
			entityid_t entityID = spriteRow.first;
			tileid_t sprite = spriteRow.second;

			if (sprite.id.first == layerTileset.tileset) {

				glm::vec3 translation = getTranslation(entityID);
				glm::vec3 spriteOffset;
				auto offsetIt = gEntity.spriteOffsets.find(entityID);
				if (offsetIt != gEntity.spriteOffsets.end()) {
					spriteOffset += offsetIt->second;
				}
				translation += spriteOffset;

				translations.push_back(translation);
				ids.push_back(sprite.id.second);
			}
		}

		int batchSize = translations.size();
		assert(batchSize == ids.size());

		if (batchSize > m_buffersCount) {
			glBindBuffer( GL_ARRAY_BUFFER, m_translationBuf );
			glBufferData( GL_ARRAY_BUFFER, sizeof( glm::ivec3 ) * batchSize, NULL, GL_DYNAMIC_DRAW );
			glBindBuffer( GL_ARRAY_BUFFER, m_idBuf );
			glBufferData( GL_ARRAY_BUFFER, sizeof( GLint ) * batchSize, NULL, GL_DYNAMIC_DRAW );
			m_buffersCount = batchSize;
		}

		glBindBuffer( GL_ARRAY_BUFFER, m_translationBuf );
		glBufferSubData( GL_ARRAY_BUFFER,
						 0,
						 sizeof( glm::ivec3 ) * batchSize,
						 translations.data() );
		glBindBuffer( GL_ARRAY_BUFFER, m_idBuf );
		glBufferSubData( GL_ARRAY_BUFFER,
						 0,
						 sizeof( GLint ) * batchSize,
						 ids.data() );

		glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, batchSize );
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

//SpriteRenderer *SpriteRenderer::get() {
//	if (!m_instance) {
//		m_instance = new SpriteRenderer();
//	}
//	return m_instance;
//}

} // namespace te
