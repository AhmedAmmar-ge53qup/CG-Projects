#include "ShaderProgram.h"

#include <iostream>
#include <fstream>
#include <sstream>

ShaderProgram::ShaderProgram()
	:mHandle(0)
{}

ShaderProgram::~ShaderProgram() 
{
	// Delete the program
	glDeleteProgram(mHandle);
}


bool ShaderProgram::loadShaders(const char* vsfilename, const char* fsfilename) 
{
	string vsString = fileToString(vsfilename);
	string fsString = fileToString(fsfilename);
	const GLchar* vsSourcePtr = vsString.c_str();
	const GLchar* fsSourcePtr = fsString.c_str();

	// 1. Setup Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vsSourcePtr, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, ShaderType::VERTEX);

	// 2. Setup Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fsSourcePtr, NULL);
	glCompileShader(fragmentShader);
	checkCompileErrors(fragmentShader, ShaderType::FRAGMENT);

	// 3. Create Shader Program
	mHandle = glCreateProgram();
	if (mHandle == 0)
		printf("Error: Failed to Create Shader Program");
	glAttachShader(mHandle, vertexShader);	// Attaching Vertex Shader to the Program
	glAttachShader(mHandle, fragmentShader);	// Attaching Fragment Shader to the Program
	glLinkProgram(mHandle);
	//glValidateProgram(mHandle);
	checkCompileErrors(mHandle, ShaderType::PROGRAM);

	// 4. Clean up, because we don't need them anymore (already compiled & linked)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

void ShaderProgram::use() 
{
	if (mHandle > 0)
		glUseProgram(mHandle);
}

GLuint ShaderProgram::getProgram() const 
{
	return mHandle;
}

string ShaderProgram::fileToString(const string& filename) 
{
	std::stringstream ss;
	std::ifstream file;

	try
	{
		file.open(filename, std::ios::in);
		if (!file.fail())
			ss << file.rdbuf();
		file.close();
	}
	catch (const std::exception&)
	{
		printf("Error: Failed to Read Shader filename");
	}

	return ss.str();
}


void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
{
	GLint status;
	if (type == ShaderType::PROGRAM)
	{
		// Check for linking Errors
		glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) 
		{
			GLint length = 0;
			glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);
			string errorLog(length, ' ');
			glGetShaderInfoLog(mHandle, length, &length, &errorLog[0]);
			std::cerr << "Error: Shader Program Failed To Link.\n" << errorLog << std::endl;
		}
	}

	else // Vertex or Fragment
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) 
		{
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			string errorLog(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
			if (type == ShaderType::VERTEX)
				std::cerr << "Error: Vertex Shader Failed to Compile.\n" << errorLog << std::endl;
			else
				std::cerr << "Error: Fragment Shader Failed to Compile.\n" << errorLog << std::endl;
		}
	}

}


GLint ShaderProgram::getUniformLocation(const GLchar* name) 
{
	std::map<string, GLint>::iterator it = mUniformLocations.find(name);

	// If name was not found, add it to the map 
	// We can actually put "mUniformLocations.find(name)" directly instead of "it"
	if (it == mUniformLocations.end())
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);

	// If name was found
	return mUniformLocations[name];
}


void ShaderProgram::setUniform(const GLchar* name, const glm::vec2& v)
{
	GLint loc = getUniformLocation(name);
	if (loc == -1)
		std::cerr << "Warning: uniform \"" << name << "\" doesn't exist\n";
	glUniform2f(loc, v.x, v.y);
}
void ShaderProgram::setUniform(const GLchar* name, const glm::vec3& v)
{
	GLint loc = getUniformLocation(name);
	if (loc == -1)
		std::cerr << "Warning: uniform \"" << name << "\" doesn't exist\n";
	glUniform3f(loc, v.x, v.y, v.z);
}
void ShaderProgram::setUniform(const GLchar* name, const glm::vec4& v)
{
	GLint loc = getUniformLocation(name);
	if (loc == -1)
		std::cerr << "Warning: uniform \"" << name << "\" doesn't exist\n";
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}