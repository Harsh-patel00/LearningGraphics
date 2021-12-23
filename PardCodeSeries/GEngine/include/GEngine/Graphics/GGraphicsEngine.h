#pragma once
#include "GEngine/Math/GVec4.h"
#include "GEngine/Math/GRect.h"

class GGraphicsEngine
{
public: // Constructor/Destructor
	GGraphicsEngine();
	~GGraphicsEngine();

public: // Member functions (Helpers)
	GVertexArrayObjectPtr CreateVertexArrayObject(const GVertexBufferData&);

public: // Member functions for OpenGL
	void Clear(const GVec4 &color);
	void SetViewport(const GRect& size);
	void SetVertexArrayObj(const GVertexArrayObjectPtr& vao);
	void DrawTriangle(ui32 vertexCount, ui32 offset);
};

