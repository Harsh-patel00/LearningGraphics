#pragma once

#include "GEngine/GPrerequisites.h"

class GVertexArrayObject
{
public: // Constructor/Destructor
	GVertexArrayObject(const GVertexBufferData&);
	~GVertexArrayObject();

public: // Member functions
	ui32 GetId();
	ui32 GetVertexBufferSize();
	ui32 GetVertexSize();

private: // Member variables
	ui32 _vertexBufferId{};
	ui32 _vertexArrayObjectId{};
	GVertexBufferData _vertexBufferData;
};

