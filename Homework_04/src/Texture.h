#ifndef TEXTURE_H
#define TEXTURE_H

#define GLEW_STATIC
#include "GL/glew.h"
#include <string>
using namespace std;

class Texture
{
public:
	Texture();
	~Texture();

	bool loadTexture(string &fileName, bool generateMipMap = true);
	bool bind(GLuint texUnit = 0);
	bool unbind(GLuint texUnit = 0);

private:
	GLuint mTexture;

};

#endif //	TEXTURE_H

