#pragma once

#include <string>
#include "GEngine/GPrerequisites.h"

class GShaderProgram
{
private:
	ui32 _programId = 0;
	ui32 _attachedShaders[2] = {};

public:
	GShaderProgram(const GShaderProgramDesc& desc);
	~GShaderProgram();

public:
	void AttachShader(const wchar_t* shaderSourcePath, const GShaderType& type);
	void Link();
	ui32 GetId();
	void SetUniform(const char* uniformName, const GShaderUniformType& type, std::size_t value);

};

