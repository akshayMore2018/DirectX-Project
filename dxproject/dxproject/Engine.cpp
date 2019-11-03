#include "Engine.h"
#include <memory>
#include "Graphics.h"
#include "Input.h"
#include "Camera.h"
#include <string>

const int WIDTH = 1920;
const int HEIGHT = 1200;

Engine::Engine()
	:	mGraphics(nullptr)
	,	mInput(nullptr)
	,	Hwnd(NULL)
	,	Initialized(false)
{
	
}

Engine::~Engine()
{
	DeleteAll();
	ShowCursor(true);
}


void Engine::Initialize(HWND Hwnd)
{
	this->Hwnd = Hwnd;

	mGraphics = new Graphics();
	mGraphics->initialize(Hwnd, WIDTH, HEIGHT);

	mInput = new Input();
	mInput->initialize(Hwnd, WIDTH, HEIGHT);

	Initialized = true;

}


void Engine::Run(HWND Hwnd)
{
	processInput();
	RenderGame();

}


void Engine::RenderGame()
{
	if (mGraphics!=nullptr)
		mGraphics->render();
}

void Engine::ReleaseAll()
{
}

void Engine::ResetAll()
{
}

void Engine::DeleteAll()
{
	if (mInput)
		delete mInput;
	if (mGraphics)
		delete mGraphics;
}


void Engine::ExitGame()
{
	PostMessage(Hwnd, WM_DESTROY, 0, 0);
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

		if (mInput->isKeyHeld(VK_Z))
		{
			mGraphics->camera->adjustPosition(0.0f,-cameraSpeed,0.0f);
		}

		if (mInput->isKeyHeld(VK_SPACE))
		{
			mGraphics->camera->adjustPosition(0.0f, cameraSpeed, 0.0f);
		}

		if (mInput->isRightPressed)
		{
			int x, y;
			mInput->getMouseRawLocation(x,y);
			mGraphics->camera->adjustRotation((float)y*0.01, (float)x*0.01, 0.0f);
		}

		if (mInput->isKeyReleased(VK_ESCAPE))
		{
			PostMessage(this->Hwnd, WM_CLOSE, 0, 0);
		}
	}
}

LRESULT Engine::MessageHandler(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam)
{
	if (Initialized)
	{
		switch (Message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);		//Terminate this program.
			return 0;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:

			return 0;
		case WM_KEYUP:
		case WM_SYSKEYUP:

			return 0;
		case WM_CHAR:				// Character entered
			return 0;
		case WM_MOUSEMOVE:
			return 0;
		case WM_INPUT:				//raw mouse data
		{
			UINT dataSize;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(LParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
			if (dataSize > 0)
			{
				std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dataSize);
				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(LParam), RID_INPUT, rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
				{
					RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());
					if (raw->header.dwType == RIM_TYPEMOUSE)
					{
						mInput->onMouseRawMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
					}
				}

			}

		}
			return DefWindowProc(Hwnd, Message, WParam, LParam);
		case WM_LBUTTONDOWN:
		{
			int x = LOWORD(LParam);
			int y = HIWORD(LParam);
			mInput->onLeftMousePressed(x, y);

		}
			return 0;
		case WM_LBUTTONUP:
		{
			int x = LOWORD(LParam);
			int y = HIWORD(LParam);
			mInput->onLeftMouseReleased(x, y);

		}
			return 0;
		case WM_MBUTTONDOWN:
			return 0;
		case WM_MBUTTONUP:
			return 0;
		case WM_RBUTTONDOWN:
		{
			SetCapture(Hwnd);
			int x = LOWORD(LParam);
			int y = HIWORD(LParam);
			mInput->onRightMousePressed(x, y);

		}
			return 0;
		case WM_RBUTTONUP:
		{
			ReleaseCapture();
			int x = LOWORD(LParam);
			int y = HIWORD(LParam);
			mInput->onRightMouseReleased(x, y);

		}
			return 0;
		case WM_DEVICECHANGE:
			return 0;
		}
	}

	return DefWindowProc(Hwnd, Message, WParam, LParam);

}


Graphics * Engine::GetGraphics()
{
	return mGraphics;
}

Input * Engine::GetInput()
{
	return mInput;
}
