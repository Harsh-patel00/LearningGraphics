#pragma once

#include<memory>

class GVertexArrayObject;


using GVertexArrayObjectPtr = std::shared_ptr<GVertexArrayObject>;
using f32 = float;
using i32 = int;
using ui32 = unsigned int;

struct GVertexBufferData
{
	void* verticesList = nullptr;
	ui32 vertexSize{};
	ui32 listSize{};
};
