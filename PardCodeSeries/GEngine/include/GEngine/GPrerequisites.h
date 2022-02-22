#pragma once

#include<memory>

class GVertexArrayObject;
class GShaderProgram;


using GVertexArrayObjectPtr = std::shared_ptr<GVertexArrayObject>;
using GShaderProgramPtr = std::shared_ptr<GShaderProgram>;

using f32 = float;
using i32 = int;
using ui32 = unsigned int;

struct GVertexAttribute
{
	ui32 numElements = 0;
};

struct GVertexBufferDesc
{
	void* verticesList = nullptr;
	ui32 vertexSize{};
	ui32 listSize{};

	GVertexAttribute* attributesList = nullptr;
	ui32 attributesListSize = 0;
};

struct GShaderProgramDesc
{
	const wchar_t* vertexShaderPath;
	const wchar_t* fragmentShaderPath;
};

enum class GShaderType
{
	vertex = 0,
	fragment = 1
};

enum class GShaderUniformType
{
	Matrix4f,

	Vec4f,
	Vec3f,
	Vec2f,

	Vec4i,
	Vec3i,
	Vec2i
};
