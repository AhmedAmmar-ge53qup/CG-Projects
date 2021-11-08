#include "Cube.h"
#include "imgui/imgui.h"


Cube::Cube()
	:model(glm::mat4(1.0f))
{
	setupCube();
}

void Cube::Draw(ShaderProgram& shaderProgram)
{
	glBindVertexArray(VAO);
	shaderProgram.use();
	shaderProgram.setUniform("model", model);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(GLuint));

	glBindVertexArray(0);	// Cleanup
}

void Cube::setupCube() {
	glGenBuffers(1, &VBO);		// Generate Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		// Set it as working buffer (State Machine)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);		// Set it as the active one (State Machine)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr); // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); //Textures
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);	// Cleanup
}
