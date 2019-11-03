#include <Windows.h>
#include "Constants.h"
#include "Engine.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HRESULT CreateMainWindow(HWND &Hwnd,HINSTANCE HInstance, const std::string& EngineWindowClassName, int Width, int Height);

LRESULT CALLBACK WindowProc(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam);

HWND Hwnd = NULL;

Engine* _Engine = nullptr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void Safe_Delete(T* &Object) //param : reference to a pointer
{
	delete Object;
	Object = nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI wWinMain(HINSTANCE HInstance, HINSTANCE PrevIntance,
	LPWSTR CmdLine, int CmdShow)
{
	_Engine = new Engine();

	HRESULT Result = CreateMainWindow(Hwnd, HInstance, "DirectXWindow", WINDOW_WIDTH, WINDOW_HEIGHT);
	if (FAILED(Result))
	{
		return -1;
	}

	try
	{
		_Engine->Initialize(Hwnd);

		MSG Message;
		ZeroMemory(&Message, sizeof(MSG));
		bool Running = true;

		while (Running)
		{
			if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
			{
				if (Message.message == WM_QUIT || Message.message == WM_NULL)
				{
					if (!IsWindow(Hwnd))
					{
						Hwnd = NULL;
						UnregisterClass((LPCSTR)"DirectXWindow", HInstance);
						Running = false;
					}
				}

				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
			else
			{
				_Engine->Run(Hwnd);
			}

		}
		Safe_Delete(_Engine);
		return Message.wParam;
	}
	catch (...)
	{
		_Engine->DeleteAll();
		DestroyWindow(Hwnd);
		MessageBox(NULL, "Unknown error occured in game.", "Error", MB_OK);
	}
	Safe_Delete(_Engine);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam)
{
	return _Engine->MessageHandler(Hwnd, Message, WParam, LParam);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HRESULT CreateMainWindow(HWND &Hwnd, HINSTANCE HInstance, const std::string& EngineWindowClassName, int Width, int Height)
{
	HRESULT Hr = S_OK;
	if (HInstance == NULL)
		HInstance = (HINSTANCE)GetModuleHandle(NULL);

	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = HInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = EngineWindowClassName.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		DWORD error = GetLastError();
		if (error != ERROR_CLASS_ALREADY_EXISTS)
			return HRESULT_FROM_WIN32(error);
	}

	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - Width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - Height / 2;
	RECT wr;
	wr.left = centerScreenX;
	wr.top = centerScreenY;
	wr.right = wr.left + Width;
	wr.bottom = wr.top + Height;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);


	Hwnd = CreateWindowEx(0,
		EngineWindowClassName.c_str(),
		EngineWindowClassName.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		wr.left,
		wr.top,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		HInstance,
		NULL);

	if (!Hwnd)
		return S_FALSE;

	ShowWindow(Hwnd, SW_SHOW);
	SetForegroundWindow(Hwnd);
	SetFocus(Hwnd);

	return Hr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
