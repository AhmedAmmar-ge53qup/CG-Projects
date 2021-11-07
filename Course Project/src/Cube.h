#ifndef CUBE_H
#define CUBE_H

#include "glm/glm.hpp"
#include "ShaderProgram.h"

class Cube
{
public:
	Cube(GLfloat vertices[]);
	void Draw(ShaderProgram& shaderProgram);
	glm::mat4 model;

private:
	GLfloat vertices[36];
};
#endif	//CUBE_H

