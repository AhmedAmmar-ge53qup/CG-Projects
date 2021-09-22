#include "Texture.h"

Texture::Texture()
	:mTexture(0)
{
}
Texture::~Texture() {
	glDeleteTextures(1, &mTexture);
}

bool Texture::loadTexture(string& fileName, bool generateMipMap) {

}
bool Texture::bind(GLuint texUnit = 0) {

}
