#ifndef CUBE_H
#define CUBE_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "GLTexture.h"


class Cube
{
public:
	Cube();
	void Draw(ShaderProgram& shaderProgram);
	void setupCube();
	glm::mat4 model;

private:
	GLTexture texture;
	GLuint VAO, VBO;
	GLfloat vertices[288] = {
		// Position				// Normal				// Tex Coords

		//front face
		-1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	 0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	 1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,	 0.0f,  0.0f,  1.0f,	 1.0f, 0.0f,

		 //back face				    
		-1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,	 0.0f,  0.0f, -1.0f,	 1.0f, 0.0f,

		 //left face				    	   
		-1.0f,  1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,	-1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,	-1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,

		//right face				    	   
		 1.0f,  1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f,	 1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,	 1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,

		//top face					    	   
	    -1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	 0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	 1.0f, 1.0f,
	    -1.0f,  1.0f, -1.0f,	 0.0f,  1.0f,  0.0f,	 0.0f, 1.0f,
	    -1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	 0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f,	 0.0f,  1.0f,  0.0f,	 1.0f, 0.0f,

		//bottom face				    	   
		-1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	 0.0f, 1.0f,
		 1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	 1.0f, 0.0f,
		 1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	 1.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,	 0.0f, -1.0f,  0.0f,	 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,	 0.0f, -1.0f,  0.0f,	 1.0f, 0.0f
	};
};
#endif	//CUBE_H

