#include "Model.h"

Model::Model()
{
	std::cout << "Empty Constructor Called!" << std::endl;
}

Model::Model(std::string path)
{
	std::cout << "Loading Model..." << std::endl;
	LoadModel(path);
	std::cout << "Load Model Complete!" << std::endl;
}

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int child = 0; child < node->mNumChildren; child++)
	{
		ProcessNode(node->mChildren[child], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> meshVertices;
	std::vector<unsigned int> meshIndices;
	std::vector<STexture> meshTextures;

	// Vertex Data
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		if (mesh->mTextureCoords[0])
		{
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f);
		}

		meshVertices.push_back(vertex);
	}

	// Indices Data
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		unsigned int index;

		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			index = mesh->mFaces[i].mIndices[j];
			meshIndices.push_back(index);
		}
	}

	// Material/STexture Data
	if (mesh->mMaterialIndex >= 0)
	{
		STexture texture;

		// Retrive the material of the mesh from the index
		aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<STexture> diffuseMaps  = LoadMaterialTextures(mat, aiTextureType_DIFFUSE, "textureDiffuse");
		std::vector<STexture> specularMaps = LoadMaterialTextures(mat, aiTextureType_SPECULAR, "textureSpecular");

		meshTextures.insert(meshTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
		meshTextures.insert(meshTextures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(meshVertices, meshIndices, meshTextures);
}

std::vector<STexture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<STexture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString texPath;
		mat->GetTexture(type, i, &texPath);

		bool skip = false;

		for (unsigned int j = 0; j < loadedTextures.size(); j++)
		{
			if (std::strcmp(loadedTextures[j].path.c_str(), texPath.C_Str()) == 0)
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			STexture texture;
			texture.id = TextureFromFile(texPath.C_Str(), directory);
			texture.type = typeName;
			texture.path = texPath.C_Str();
			textures.push_back(texture);
			loadedTextures.push_back(texture);
		}
	}


	return textures;
}

int Model::TextureFromFile(std::string path, std::string dir)
{
	std::string filename = dir + '/' + path;

	Texture tex(filename.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);

	return tex.ID;
}
