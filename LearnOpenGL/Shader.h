#pragma once

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"

class Shader
{
public:
	// Shader program ID (Unique Identifier)
	uint32_t ID{};

	// Constructor to accept vertex and fragment shader paths
	Shader(const char* vertexSourcePath, const char* fragmentSourcePath);

	// Use/Activate the shader
	void Use();

	// Utility Functions
	void SetBool(const std::string &name, bool value)	const;
	void SetInt(const std::string &name, int value)		const;
	void SetFloat(const std::string& name, float value)	const;
	void SetUniformVec4f(const std::string &name, glm::vec4) const;
	void SetUniformVec3f(const std::string &name, glm::vec3) const;
	void SetMatrixUniform4f(const std::string& name, float *value) const;

private:
	// utility function for checking shader compilation/linking errors.
	void CheckCompileErrors(unsigned int shader, std::string type);
};