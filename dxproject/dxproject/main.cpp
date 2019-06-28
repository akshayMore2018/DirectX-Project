
#include <Windows.h>
#include "Constants.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message,
	WPARAM wParam, LPARAM lParam);
bool createMainWindow(HINSTANCE hInstance , int nCmdShow);


TCHAR ch = ' ';

//used wWinMain instead of WinMain to avoid implicit conversion between Unicode and ANSI.
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	LPWSTR cmdLine, int cmdShow)
{
	//prevInstance : handle of previous instance of the application. This will always be NULL.
	//LPSTR(or LPWSTR in unicode) command line for application without the program's name.

	UNREFERENCED_PARAMETER(prevInstance); //to suppress compiler warnings about parameters that are unused by a function's body.
	UNREFERENCED_PARAMETER(cmdLine);

	if (!createMainWindow(hInstance, cmdShow))
		return -1;

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		//PeekMessage : retrieves a message for the associated window
		//PM_REMOVE remove flag removes the message from the queue.
		//Since we are processing this message, it will not need to stay on the queue once we’ve handled it.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			//translates the messages from virtual-key messages to character messages
			DispatchMessage(&msg);
			//dispatches the message to the Windows procedure callback function,
			//The Windows procedure function will actually perform actions based on the message it receives.
		}
		else
		{
			//If there are no messages, the only thing to do is to perform game updates and rendering
			//update
			//draw
		}
	}
	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC hDC;
	switch (message)
	{
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &paintStruct);
		RECT rc;
		GetClientRect(hwnd, &rc);
		TextOut(hDC, rc.right / 2, rc.bottom / 2, &ch, 1);
		EndPaint(hwnd, &paintStruct);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 0x08://BackSpace
		case 0x09://Tab
		case 0x0A://Linefeed
		case 0x0D://Carriage return
		case 0x1B://Escape
			MessageBeep((UINT)-1);
			break;
		default://displayable character
			ch = (TCHAR)wParam;//Get the character
			InvalidateRect(hwnd, NULL, TRUE);//Force WM_PAINT
		}
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		//This function is only called on messages for which we are not writing custom response code.
		//DefWindowProc function is a Win32 function that processes every message sent to it with default processing behavior 
		//and is used to ensure that we at least respond to every message, even if we don’t actually care about them.
	}
	return 0;
}

bool createMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	//hInstance : handle of application's current instance
	//cmdShow : an ID that specifies how the window should be shown.

	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	//size of the structure in bytes
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	//style flags used to define wiondow's look
	wndClass.lpfnWndProc = WndProc;
	//A callback function that is called whenever an event notification comes from OS.
	//This property is a function pointer
	wndClass.hInstance = hInstance;
	// the application instance that contains the window's procedure(callback) for this window class.
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//The resource ID for the graphic that will act as the cursor.
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//A handle to the background brush that will be used for painting the window's background.
	wndClass.lpszMenuName = NULL;
	//A null-terminated string of the resource name for the menu.
	wndClass.lpszClassName = CLASS_NAME;
	//A null-terminated string for what you wish to name your window class. (max 256 characters)


	if (!RegisterClassEx(&wndClass))
		return false;
	//window class structure created is send to RegisterClassEx() to register the window class.
	//The RegisterClassEx() must be called before we attempt to create the window.
	//if returned 0 then registeration failed.

	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	//calculates the size required of the window based on our desired dimensions and style.
	//we have both client and non-client(title bar , border etc) areas

	HWND hwnd = CreateWindowA(CLASS_NAME, APP_TITLE,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	//used CreateWindowA instead of CreateWindow to accept ANSI string parameters instead of Unicode.
	//lpClassName : window class structure name
	//lpWindowName : window title bar text
	//dwStyle : window's style flags
	//x : windows's horizontal position.
	//y : windows's vertical position.
	//nWidth : window width
	//hHeight : window height
	//hWndParent : handle to the parent window's handle.
	//hMenu : Resource handle to the window's menu.
	//hInstance : application instance ID
	//lpParam : Data to be passed to the window and made available via the lpParam parameter of windows proc callback function.

	if (!hwnd)
		return false;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return true;
}
