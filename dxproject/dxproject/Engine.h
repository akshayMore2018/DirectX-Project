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
	void processInput();
	void update();
	void render();
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	HRESULT initializeWindow(int width,int height);
	HRESULT initializeGraphics();
	HRESULT initializeInput();

	HWND handle;
	HINSTANCE hInstance;
	std::string mEngineWindowClassName;
	class Graphics * mGraphics;
	class Input* mInput;
};
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static Engine* engineHandle = 0;
