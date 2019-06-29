#include "System.h"
#include "BlankDemo.h"
System::System() :m_Graphics(nullptr)
{
}

System::~System()
{
}

bool System::initialize()
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	result = initializeWindow(screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, "Counldnt initialize window", "Window ERROR", MB_OK);
		return false;
	}

	m_Graphics = new BlankDemo();

	result = m_Graphics->initialize(hInstance,hwnd,false);
	if (!result)
	{
		MessageBox(hwnd, "Counldnt initialize graphics", "Graphic ERROR", MB_OK);
		return false;
	}
	return true;
}

void System::run()
{
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
				m_Graphics->update(0.0f);
				m_Graphics->render();

				
			}
		}
}

void System::release()
{
	if (m_Graphics)
	{
		m_Graphics->releaseAll();
		delete m_Graphics;
		m_Graphics = 0;
	}

	//same for input class

	releaseWindow();

}

LRESULT System::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		case WM_KEYDOWN:

			return 0;
		case WM_KEYUP:

			return 0;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
			//		//This function is only called on messages for which we are not writing custom response code.
			//		//DefWindowProc function is a Win32 function that processes every message sent to it with default processing behavior 
			//		//and is used to ensure that we at least respond to every message

	}
}

bool System::initializeWindow(int &, int &)
{

	ApplicationHandle = this;
	hInstance = GetModuleHandle(NULL);
	m_applicationName =(LPCWSTR)"DemoFrameWork";

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
	wndClass.lpszClassName = (LPCSTR)m_applicationName;
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
	
	hwnd = CreateWindowA((LPCSTR)m_applicationName, (LPCSTR)m_applicationName,
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
	
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	
	return true;

}

void System::releaseWindow()
{
	DestroyWindow(hwnd);
	hwnd = NULL;
	UnregisterClass((LPCSTR)m_applicationName, hInstance);
	hInstance = NULL;

	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return ApplicationHandle->MessageHandler(hwnd, message, wParam, lParam);
	}
}
