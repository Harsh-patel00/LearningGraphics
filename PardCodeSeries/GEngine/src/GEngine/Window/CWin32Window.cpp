#include <assert.h>
#include <corecrt_wstdio.h>

#include "GEngine/Window/GWindow.h"
#include "GEngine/Console/GConsole.h"
#include "glad/glad_wgl.h"
#include "glad/glad.h"

// Handle window 'procedure' (This function is a callback)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// Sent when a window is being destroyed. 
		// It is sent to the window procedure of the window being destroyed after the window is removed from the screen.
		case WM_DESTROY:
		{

			// Get the reference of the window that is currently being processed
			// GWindow* window = (GWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			break;
		}
		case WM_CLOSE:
		{
			// Indicates to the system that a thread has made a request to terminate(quit).
			// It is typically used in response to a WM_DESTROY message
			PostQuitMessage(0);
			break;
		}
		case WM_KEYDOWN:

			switch (wParam)
			{
				// Check if Esc key is pressed, if so then close the window
				case VK_ESCAPE:
					GConsole::LOG("GWindow::LOG::Closing window...");
					PostQuitMessage(0);
					break;
				default:
					GConsole::KEYLOG((CHAR)wParam);
					break;
			}

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return NULL;
}

GWindow::GWindow()
{
	#pragma region Create and Show Window

	// Class related to specific window properties
	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"MyWindow"; // Identifier of the window
	wc.lpfnWndProc = &WndProc;
	wc.style = CS_OWNDC;

	// Registering our window, by passing it's address
	auto classId = RegisterClassEx(&wc);
	
	// Check if classId is null or not
	assert(classId);

	// Define the rectangle of window (Actual draw area)
	RECT rc = { 0, 0, 800, 600 };

	// Define window RECT properties
	AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, false);

	// Create the window
	// 1. WS_OVERLAPPED : Overlapped window
	// 2. WS_CAPTION	: Title bar 
	// 3. WS_SYSMENU	: Window menu (When we click on title bar)
	// 4. CW_USEDEFAULT	: Uses default settings
	_handle = CreateWindowEx(NULL, MAKEINTATOM(classId), L"Gagged Engine", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, NULL, NULL);

	// Check if the window is created or not
	assert(_handle);

	// Get the window instance from window procedure (WndProc)
	SetWindowLongPtr((HWND)_handle, GWLP_USERDATA, (LONG_PTR)this);

	// Show Window
	ShowWindow((HWND)_handle, SW_SHOW);
	UpdateWindow((HWND)_handle);

	#pragma endregion

	#pragma region Create OpenGL render context
	auto hDC = GetDC(HWND(_handle));

	int pixelFormatAttributes[] = 
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0
	};

	int pixelFormat = 0;
	UINT numFormats = 0;
	wglChoosePixelFormatARB(hDC, &pixelFormatAttributes[0], nullptr, 1, &pixelFormat, &numFormats);
	assert(numFormats);

	PIXELFORMATDESCRIPTOR pixelFormatDesc = {};
	DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDesc);
	SetPixelFormat(hDC, pixelFormat, &pixelFormatDesc);

	int openGLAttributes[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	_context = wglCreateContextAttribsARB(hDC, 0, openGLAttributes);
	assert(_context);

	#pragma endregion
}

GWindow::~GWindow()
{
	wglDeleteContext((HGLRC)_context);
	// DestroyWindow((HWND)_handle);
}

// Set our created window as current context
void GWindow::MakeCurrentContext()
{
	wglMakeCurrent(GetDC(HWND(_handle)), (HGLRC)_context);
}

// Swap buffers to smoothly draw on window
void GWindow::Present(bool vSync)
{
	wglSwapIntervalEXT(vSync);
	wglSwapLayerBuffers(GetDC(HWND(_handle)), WGL_SWAP_MAIN_PLANE);
}

GRect GWindow::GetInnerSize()
{
	RECT rc{};
	// This funtion take 'IN' HWND window, and 'OUT'put the rect in 'rc' parameter
	GetWindowRect((HWND)_handle, &rc);

	return GRect(rc.right-rc.left, rc.bottom-rc.top);
}
