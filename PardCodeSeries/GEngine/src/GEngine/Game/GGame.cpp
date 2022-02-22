#include "GEngine/Game/GGame.h"
#include "GEngine/Window/GWindow.h"
#include "GEngine/Graphics/GGraphicsEngine.h"
#include "GEngine/Graphics/GVertexArrayObject.h"


GGame::GGame()
{
	// Create a new graphics engine on start of the game
	// Always create this first, because, it creates OpenGL context
	_graphicsEngine = std::make_unique<GGraphicsEngine>();

	// Create a new window on start of the game
	_display = std::make_unique<GWindow>();
	
	// Set this game as our current context
	_display->MakeCurrentContext();

	// Set the viewport for OpenGL
	_graphicsEngine->SetViewport(_display->GetInnerSize());
}

GGame::~GGame()
{}

void GGame::OnCreate()
{
	const f32 triangleVertices[] =
	{
		// Top
		0.f, 0.5f, 0.f,
		0, 0, 1, // Green

		// Bottom-Left
		-0.5f, -0.5f, 0.f,
		1, 0, 0, // Red

		// Bottom-Right
		0.5f, -0.5f, 0.f,
		0, 1, 0 // Blue

	};

	GVertexAttribute attribsList[]
	{
		3,	// Position
		3	// Color
	};

	_triangleVAO = _graphicsEngine->CreateVertexArrayObject(
		{
			(void*)triangleVertices, 
			sizeof(f32)*(3+3),
			3, 
			attribsList, 
			2
		});

	_triangleShader = _graphicsEngine->CreateShader({ L"Shaders/Vertex/triangleVert.vert", 
		L"Shaders/Fragment/triangleFrag.frag"});
}

void GGame::OnUpdate()
{
	_graphicsEngine->Clear(GVec4(0.2, 0.2, 0.2, 1));

	_graphicsEngine->SetVertexArrayObj(_triangleVAO);

	_graphicsEngine->SetShader(_triangleShader);

	_graphicsEngine->DrawTriangle(_triangleVAO->GetVertexBufferSize(), 0);

	// Always at end (Double buffering)
	_display->Present(false);
}

void GGame::OnQuit()
{
}

void GGame::Quit()
{
	_isRunning = false;
}
