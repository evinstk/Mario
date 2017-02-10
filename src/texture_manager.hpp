#pragma once
#include <GL/glew.h>
#include <EASTL/hash_map.h>
#include <EASTL/string.h>
#include <tegl/types.hpp>

namespace te {

class TextureManager {
public:
	GLuint load(const char *pathname);
private:
	eastl::hash_map<eastl::string, Texture> m_textureMap;
};

extern TextureManager *gTextureManager;

}
