#include "Input.h"

Input::Input()
	:mInput(0),mMouse(0)
{
	ZeroMemory(keys_, sizeof(keys_));
}

Input::~Input()
{
	if (mMouse)
	{
		mMouse->Unacquire();
		mMouse->Release();
		mMouse = 0;
	}

	if (mInput)
	{
		mInput->Release();
		mInput = 0;
	}

}

HRESULT Input::initialize(HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result = S_OK;
	this->mScreenHeight = screenHeight;
	this->mScreenWidth = screenWidth;
	mMouseX = 0;
	mMouseY = 0;

	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);

	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mInput, NULL);
	if (FAILED(result))
	{
		return result;
	}

	result = mInput->CreateDevice(GUID_SysMouse, &mMouse, NULL);
	if (FAILED(result))
	{
		return result;
	}

	result = mMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	result = mMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return result;
	}
	result = mMouse->Acquire();
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

void Input::update()
{
	bool result;
	ZeroMemory(mKeyboardState, sizeof(mKeyboardState));
	result = readKeyboard();
	if (!result)
	{
		return;
	}
	result = readMouse();
	if (!result)
	{
		return;
	}
	processInput();
}

bool Input::isKeyPressed(int virtualKey) const
{
	return (keys_[virtualKey] & KEY_STATE_PRESSED) != 0;
}

bool Input::isKeyHeld(int virtualKey) const
{
	return (keys_[virtualKey] & KEY_STATE_HELD) != 0;
}

bool Input::isKeyReleased(int virtualKey) const
{
	return (keys_[virtualKey] & KEY_STATE_RELEASED) != 0;
}

unsigned char Input::getKeyState(int virtualKey) const
{
	return keys_[virtualKey];
}

bool Input::readKeyboard()
{
	if (GetKeyboardState(mKeyboardState))
	{
		return true;
	}
	return false;
}

bool Input::readMouse()
{
	HRESULT result;
	result = mMouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mMouseState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mMouse->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}

void Input::processInput()
{
	const unsigned char HIGH_BIT = 1 << 7;
	for (int i = VK_LBUTTON; i <= VK_OEM_CLEAR; i++)
	{
		bool isDown = (mKeyboardState[i] & HIGH_BIT) != 0;
		bool wasDown = (keys_[i] & KEY_STATE_HELD) != 0;
		unsigned char newState = 0;

		if (isDown)
		{
			newState |= KEY_STATE_HELD;
		}

		if (isDown && !wasDown)
		{
			newState |= KEY_STATE_PRESSED;
		}
		if (!isDown && wasDown)
		{
			newState |= KEY_STATE_RELEASED;
		}

		keys_[i] = newState;

	}
}
