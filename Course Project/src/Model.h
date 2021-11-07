#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Model
{

public:
	Model() {}
	Model(string const &path)
	{
		loadModel(path);
	}
	void loadModel(string path);
	void Draw(ShaderProgram& shader);


private:
	// model data
	std::vector<Mesh> meshes;
	std::string directory;
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	// Helper function
	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

	// Optimization
	std::vector<Texture> textures_loaded;


};

#endif // MODEL_H

