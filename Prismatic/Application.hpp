#ifndef _APPLICATION_HPP
#define _APPLICATION_HPP

#include "PCH.hpp"

#include "DX.hpp"
#include "InputHandler.hpp"

#pragma warning(disable : 26495)
#pragma warning(disable : 28251)

class Application;
class InputHandler;

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class Application
{
public:

	Application(int win_width, int win_height);
	Application(const Application&);
	~Application();

	int Init();
	bool Shutdown();
	void Run();

	LRESULT __stdcall MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	int width = 0;
	int height = 0;
	int x_pos = 0;
	int y_pos = 0;
	LPCWSTR m_application_name = 0;
	HINSTANCE m_hinstance = 0;
	HWND m_hwnd = 0;
	InputHandler* m_inputHandler = 0;
	Application* m_Application = 0;

	Application();
	bool Render();
	void InitWin(int&, int&);
	void ShutdownWin();

	D3D* m_D3D;

};

static LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM);

static Application* ApplicationHandle = 0;

#endif