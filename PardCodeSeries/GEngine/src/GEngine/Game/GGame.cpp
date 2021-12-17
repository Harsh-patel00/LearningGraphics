#include "GEngine/Game/GGame.h"
#include "GEngine/Window/GWindow.h"
#include "GEngine/Graphics/GGraphicsEngine.h"


GGame::GGame()
{
	// Create a new graphics engine on start of the game
	// Always create this first, because, it creates OpenGL context
	_graphicsEngine = std::make_unique<GGraphicsEngine>();

	// Create a new window on start of the game
	_display = std::make_unique<GWindow>();

	// Set this game as our current context
	_display->MakeCurrentContext();
}

GGame::~GGame()
{}

void GGame::OnCreate()
{
}

void GGame::OnUpdate()
{
	_graphicsEngine->Clear(GVec4(0, 0, 1, 1));

	_display->Present(false);
}

void GGame::OnQuit()
{
}

void GGame::Quit()
{
	_isRunning = false;
}
