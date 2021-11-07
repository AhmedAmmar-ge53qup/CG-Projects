#ifndef MESH_H
#define MESH_H
#include <vector>
#include <iostream>
#include "glm/glm.hpp"
#include "ShaderProgram.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};
struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:


	// Mesh Data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(ShaderProgram& shader);

private:
	GLuint VAO, VBO, EBO;
	void setupMesh();

};

#endif //MESH_H

