#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

class System
{
public:
	System();
	~System();


	bool initialize();
	void run();
	void release();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	bool frame();
	bool initializeWindow(int&, int&);
	void releaseWindow();

	LPCWSTR m_applicationName;
	HINSTANCE hInstance;
	HWND hwnd;

	//InputClass* m_Input;
	class Graphics* m_Graphics;
};
static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static System* ApplicationHandle = 0;