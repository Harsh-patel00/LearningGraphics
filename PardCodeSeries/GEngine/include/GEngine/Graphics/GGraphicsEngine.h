#pragma once
#include "GEngine/Math/GVec4.h"
#include "GEngine/Math/GRect.h"

class GGraphicsEngine
{
public: // Constructor/Destructor
	GGraphicsEngine();		// Def in CWin32GraphicsEngine
	~GGraphicsEngine();		// Def in CWin32GraphicsEngine

public: // Member functions (Helpers)
	GVertexArrayObjectPtr CreateVertexArrayObject(const GVertexBufferDesc&);	// Def in GGraphicsEngine
	GShaderProgramPtr CreateShader(const GShaderProgramDesc& desc);					// Def in GGraphicsEngine

public: // Member functions for OpenGL
	void Clear(const GVec4 &color);								// Def in GGraphicsEngine
	void SetViewport(const GRect& size);						// Def in GGraphicsEngine
	void SetVertexArrayObj(const GVertexArrayObjectPtr& vao);	// Def in GGraphicsEngine
	void SetShader(const GShaderProgramPtr& shader);	// Def in GGraphicsEngine
	void DrawTriangle(ui32 vertexCount, ui32 offset);			// Def in GGraphicsEngine
};

