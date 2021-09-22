#include "Texture.h"
#include <iostream>
// Image Loader
#define STB_IMAGE_IMPLEMENTATION  // Required by the library
#include "stb_image/stb_image.h"
Texture::Texture()
	:mTexture(0)
{
	glGenTextures(1, &mTexture);
}
Texture::~Texture() {
	glDeleteTextures(1, &mTexture);
}

bool Texture::loadTexture(string& fileName, bool generateMipMap)
{
	int width, height, nrOfComponents;
	unsigned char* texelData = stbi_load(fileName.c_str(), &width, &height, &nrOfComponents, 0);
	if (!texelData) {
		std::cerr << "Failed to Load Image File: " << fileName << std::endl;
		return false;
	}

	GLenum format = 0;
	if (nrOfComponents == 0)
		format = GL_RED;
	if (nrOfComponents == 3)
		format = GL_RGB;
	if (nrOfComponents == 4)
		format == GL_RGBA;

	// Create Textures
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, texelData); // 2nd GL_RGB would be GL_RGBA for png files
	if(generateMipMap)
		glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(texelData);

	return true;
}
bool Texture::bind(GLuint texUnit) 
{
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glActiveTexture(texUnit);
	return true;
}
