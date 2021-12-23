#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct STexture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	// Mesh data
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<STexture>		textures;

	// Constructors
	Mesh(std::vector<Vertex> v, std::vector<unsigned int> i, std::vector<STexture> t);

	// Member Functions
	void Draw(Shader& shader);

private:

	// Render Data
	unsigned int VAO, VBO, EBO;

	void SetupMesh();
};

