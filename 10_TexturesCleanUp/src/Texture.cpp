#include "Texture.h"

Texture::Texture()
	:mTexture(0)
{
}
Texture::~Texture() {
	glDeleteTextures(1, &mTexture);
}

bool Texture::loadTexture(string& fileName, bool generateMipMap) {

	return false;
}
bool Texture::bind(GLuint texUnit) {

	return false;
}
