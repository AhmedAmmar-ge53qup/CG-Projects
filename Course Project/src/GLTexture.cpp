#include "GLTexture.h"
#include <iostream>
// Image Loader
#include "stb_image/stb_image.h"
GLTexture::GLTexture()
	:mTexture(0)
{
	//glGenTextures(1, &mTexture);
}
GLTexture::~GLTexture() {
	glDeleteTextures(1, &mTexture);
}

bool GLTexture::loadTexture(string& fileName, bool generateMipMap)
{
	int width, height, nrOfComponents;

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	// Set Texture Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// Set Wrapping Filter for s
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);		// Set Wrapping Filter for t
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Set Filter when you are far from the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Set Filter when you "magnify" the texture (get close)

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
		format = GL_RGBA;

	// Create Textures
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texelData); // 2nd GL_RGB would be GL_RGBA for png files
	if(generateMipMap)
		glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(texelData);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
bool GLTexture::bind(GLuint texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);		// We have to activate first
	glBindTexture(GL_TEXTURE_2D, mTexture);
	return true;
}
