#include <fstream>
#include <sstream>
#include <vector>

#include "GEngine/Graphics/GShaderProgram.h"
#include "GEngine/Console/GConsole.h"

#include "glad/glad.h"

GShaderProgram::GShaderProgram(const GShaderProgramDesc& desc)
{
	_programId = glCreateProgram();
	AttachShader(desc.vertexShaderPath, GShaderType::vertex);
	AttachShader(desc.fragmentShaderPath, GShaderType::fragment);
	Link();
}

GShaderProgram::~GShaderProgram()
{
	for (ui32 i = 0; i < 2; i++)
	{
		glDetachShader(_programId, _attachedShaders[i]);
		glDeleteShader(_attachedShaders[i]);
	}
	glDeleteProgram(_programId);
}

void GShaderProgram::AttachShader(const wchar_t* shaderSourcePath, const GShaderType& type)
{
	std::string shaderCode;
	std::ifstream shaderStream(shaderSourcePath);
	std::wstring shaderWstrPath(shaderSourcePath);
	std::string shaderPathStr(shaderWstrPath.begin(), shaderWstrPath.end());

	if (shaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << shaderStream.rdbuf();
		shaderCode = sstr.str();
		shaderStream.close();
	}
	else
	{
		std::ostringstream msg;
		msg << "GShaderProgram::" << shaderPathStr << " not found!";
		GConsole::WARNING(msg.str());
		return;
	}

	ui32 shaderId = 0;
	switch (type)
	{
	case GShaderType::vertex:
		shaderId = glCreateShader(GL_VERTEX_SHADER);
		break;
	case GShaderType::fragment:
		shaderId = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	default:
		break;
	}

	auto sourcePointer = shaderCode.c_str();
	glShaderSource(shaderId, 1, &sourcePointer, NULL);
	glCompileShader(shaderId);

	i32 logLength = 0;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		std::vector<char> errorMessage(logLength + 1);
		glGetShaderInfoLog(shaderId, logLength, NULL, &errorMessage[0]);
		std::ostringstream msg;
		msg << "GShaderProgram::SHADER_COMPILATION_ERROR::" << "\n" << &errorMessage[0];
		GConsole::LOGERROR(msg.str());
		return;
	}

	glAttachShader(_programId, shaderId);
	_attachedShaders[(int)type] = shaderId;

	std::ostringstream msg;
	msg << "GShaderProgram::SHADER_COMPILATION_SUCESS : " << shaderPathStr;
	GConsole::SUCCESS(msg.str());
}

void GShaderProgram::Link()
{
	glLinkProgram(_programId);

	i32 logLength = 0;
	glGetShaderiv(_programId, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		std::vector<char> errorMessage(logLength + 1);
		glGetShaderInfoLog(_programId, logLength, NULL, &errorMessage[0]);
		std::ostringstream msg;
		msg << "GShaderProgram::SHADER_LINK_ERROR" << "\n" << &errorMessage[0];
		GConsole::LOGERROR(msg.str());
		return;
	}
}

ui32 GShaderProgram::GetId()
{
	return _programId;
}

void GShaderProgram::SetUniform(const char* uniformName, const GShaderUniformType& type, std::size_t value)
{
	switch (type)
	{
	case GShaderUniformType::Vec2f:
		break;
	default:
		break;
	}
}
