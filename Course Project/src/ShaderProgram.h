//----------------------
/// <summary>
/// GLSL shader program
/// </summary>

#define GLEW_STATIC
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <string>
#include <map>

#ifndef SHADER_H
#define SHADER_H

using std::string;
class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	enum class ShaderType
	{
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	bool loadShaders(const char* vsfilename, const char* fsfilename);	// Read Shaders from File
	void use();
	GLuint getProgram() const;

	void setUniform(const GLchar* name, const glm::vec2& v);
	void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, const glm::vec4& v);
	void setUniform(const GLchar* name, const glm::mat4& m);

	void setUniform(const GLchar* name, const unsigned int v);

private:
	GLuint mHandle;		// Shader Program Handle
	string fileToString(const string& filename);
	void checkCompileErrors(GLuint shader, ShaderType type);

	GLint getUniformLocation(const GLchar* name);
	std::map<string, GLint> mUniformLocations;
};

#endif // !SHADER_H
