#pragma once

#include "GEngine/GPrerequisites.h"

class GWindow;
class GGraphicsEngine;
class GGame
{
protected:
	// Flag is true while game is running,
	// it'll be set to false when user quit's the game
	bool _isRunning = true;

	// A pointer to our graphics engine
	std::unique_ptr<GGraphicsEngine> _graphicsEngine;

	// A pointer to our window class(containing handle to our created window),
	// which helps creating a new window from our game!
	std::unique_ptr<GWindow> _display;

	GVertexArrayObjectPtr _triangleVAO{};
	GShaderProgramPtr _triangleShader{};

public:
	GGame();	// Def in GGame
	~GGame();	// Def in GGame

public:

	virtual void OnCreate();	// Def in GGame
	virtual void OnUpdate();	// Def in GGame
	virtual void OnQuit();		// Def in GGame

	void Run();		// Def in CWin32Game
	void Quit();	// Def in GGame

};

