#pragma once

#include <Windows.h>

class GWindow
{
private:

	// This is the handle to our window
	// It's type is void, and it's a pointer to our window
	// This means that it'll store the address of our window
	void* _handle = nullptr;

	// Pointer to created OpenGL context
	void* _context = nullptr;

public:
	GWindow();
	~GWindow();

public:
	void MakeCurrentContext();
	void Present(bool vSync);

};

