#include "Cube.h"

Cube::Cube(GLfloat vertices[])
	:model(glm::mat4(1.0f))
{
	for (int i = 0; i < 36; i++)
		this->vertices[i] = vertices[i];
}

void Cube::Draw(ShaderProgram& shaderProgram) {
	glDrawArrays(GL_TRIANGLES, 0, 36);
	shaderProgram.setUniform("model", model);
}
