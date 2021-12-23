#pragma once


#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Texture.h"


class Model
{
public:

	Model();
	Model(std::string path);

	void Draw(Shader &shader);

private:

	// Model Data
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<STexture> loadedTextures;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<STexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	int TextureFromFile(std::string path, std::string dir);
};

