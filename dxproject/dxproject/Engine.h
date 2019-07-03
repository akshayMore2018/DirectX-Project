#pragma once
#include <Windows.h>
#include <iostream>
class Engine
{
public:
	Engine();
	~Engine();
	bool initialize();
	bool running();
	void update();
	void render();
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	HRESULT initializeWindow(int width,int height);
	HRESULT initializeGraphics();

	HWND handle;
	HINSTANCE hInstance;
	std::string mEngineWindowClassName;
	class Graphics * mGraphics;
};
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static Engine* engineHandle = 0;
