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

public:
	GGame();
	~GGame();

public:

	virtual void OnCreate();
	virtual void OnUpdate();
	virtual void OnQuit();

	void Run();
	void Quit();

};

