#include "sprite_renderer.hpp"
#include "world_state.hpp"
#include <tegl/types.hpp>
#include <tegl/util.hpp>
#include <cassert>

namespace te {

SpriteRenderer::SpriteRenderer() {
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

void SpriteRenderer::draw(const worldstate_t& state) {
	glUseProgram(m_shader);

	glUseProgram(0);
}

//SpriteRenderer *SpriteRenderer::get() {
//	if (!m_instance) {
//		m_instance = new SpriteRenderer();
//	}
//	return m_instance;
//}

} // namespace te
