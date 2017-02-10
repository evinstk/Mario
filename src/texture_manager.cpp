#include "texture_manager.hpp"
#include <tegl/util.hpp>
#include <tegl/texture.hpp>

namespace te {

GLuint TextureManager::load(const char *pathname) {
	auto textureIt = m_textureMap.find_as(pathname);
	if (textureIt != m_textureMap.end()) {
		return textureIt->second;
	}

	Texture texture;
	GLuint textureID = texture;
	LoadIntoTexture(pathname, textureID);
	m_textureMap.insert(decltype(m_textureMap)::value_type{ eastl::string(pathname), std::move(texture) });
	return textureID;
}

static TextureManager gTextureManagerLocal;
TextureManager *gTextureManager = &gTextureManagerLocal;

} // namespace te
