#ifndef TEXTURE_H
#define TEXTURE_H

#define GLEW_STATIC
#include "GL/glew.h"
#include <string>
using namespace std;

class GLTexture
{
public:
	GLTexture();
	~GLTexture();

	bool loadTexture(string &fileName, bool generateMipMap = true);
	bool bind(GLuint texUnit = 0);

private:
	GLuint mTexture;

};

#endif //	TEXTURE_H

