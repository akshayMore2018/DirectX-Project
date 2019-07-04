#include "Engine.h"
#include <memory>
#include "Graphics.h"
#include "Input.h"
#include "Camera.h"
Engine::Engine()
	:mEngineWindowClassName("Direct3DWindowClass"),
	hInstance(NULL),
	handle(NULL),
	mGraphics(nullptr),
	mInput(nullptr)
{

}

Engine::~Engine()
{
	if (mInput)
		delete mInput;
	if (mGraphics)
		delete mGraphics;
}

bool Engine::initialize()
{
	HRESULT result = initializeWindow(800, 600);
	if (FAILED(result))
	{
		return false;
	}
	else
	{
		result = initializeGraphics();
		if (FAILED(result))
		{
			return false;
		}
		else
		{
			result = initializeInput();
			if (FAILED(result))
			{
				return false;
			}
		}
	}

	return true;
}



void Engine::update()
{
	
}

void Engine::render()
{
	if (mGraphics)
		mGraphics->render();
}

bool Engine::running()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);	
	}

	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL;
			UnregisterClass((LPCSTR)mEngineWindowClassName.c_str(), this->hInstance);
			return false;
		}
	}
	return true;
}

void Engine::processInput()
{
	if (mInput)
	{
		mInput->update();
		const float cameraSpeed = 0.02f;
		if (mInput->isKeyHeld(VK_W))
		{
			mGraphics->camera->adjustPosition(this->mGraphics->camera->getForwardVector()*cameraSpeed);
		}

		if (mInput->isKeyHeld(VK_S))
		{
			mGraphics->camera->adjustPosition(this->mGraphics->camera->getBackVector()*cameraSpeed);
		}

		if (mInput->isKeyHeld(VK_A))
		{
			mGraphics->camera->adjustPosition(this->mGraphics->camera->getLeftVector()*cameraSpeed);
		}

		if (mInput->isKeyHeld(VK_D))
		{
			mGraphics->camera->adjustPosition(this->mGraphics->camera->getRightVector()*cameraSpeed);
		}

	}



}

LRESULT Engine::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_LBUTTONUP:
		
		return 0;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

HRESULT Engine::initializeWindow(int width, int height)
{
	HRESULT hr = S_OK;
	if (this->hInstance == NULL)
		this->hInstance = (HINSTANCE)GetModuleHandle(NULL);

	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0;
	wc.hInstance = this->hInstance; 
	wc.hIcon = NULL;   
	wc.hIconSm = NULL; 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
	wc.hbrBackground = NULL; 
	wc.lpszMenuName = NULL; 
	wc.lpszClassName = mEngineWindowClassName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		DWORD error = GetLastError();
		if (error != ERROR_CLASS_ALREADY_EXISTS)
			return HRESULT_FROM_WIN32(error);
	}

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
	RECT wr;
	wr.left = centerScreenX;
	wr.top = centerScreenY;
	wr.right = wr.left + width;
	wr.bottom = wr.top + height;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);


	this->handle = CreateWindowEx(0,
		mEngineWindowClassName.c_str(),
		mEngineWindowClassName.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 
		wr.left, 
		wr.top, 
		wr.right - wr.left, 
		wr.bottom - wr.top, 
		NULL, 
		NULL,
		this->hInstance, 
		NULL); 

	if (!this->handle)
		return S_FALSE;

	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return hr;
}

HRESULT Engine::initializeGraphics()
{
	HRESULT hr = S_OK;

	mGraphics = new Graphics();

	hr = mGraphics->initialize(this->handle, 800, 600);

	return hr;
}

HRESULT Engine::initializeInput()
{
	HRESULT hr = S_OK;

	mInput = new Input();

	hr = mInput->initialize(this->handle, 800, 600);

	return hr;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	default:
		return engineHandle->MessageHandler(hwnd, uMsg, wParam, lParam);
	}
}
