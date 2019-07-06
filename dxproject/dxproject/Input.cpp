#include "Input.h"

Input::Input():isLeftPressed(false),isRightPressed(false),isMiddlePressed(false)
{
	ZeroMemory(keys_, sizeof(keys_));
}

Input::~Input()
{

}

HRESULT Input::initialize(HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result = S_OK;
	this->mScreenHeight = screenHeight;
	this->mScreenWidth = screenWidth;
	x = 0;
	y = 0;

	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);

	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02;
	rid.dwFlags = 0;
	rid.hwndTarget = NULL;

	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
	{
		result = S_FALSE;
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

void Input::onLeftMousePressed(int x, int y)
{
	this->x = x;
	this->y = y;
	this->isLeftPressed = true;
}

void Input::onRightMousePressed(int x, int y)
{
	/*this->x = x; because conflicting with raw mouse position
	this->y = y;*/
	this->isRightPressed = true;
}

void Input::onMiddleMousePressed(int x, int y)
{
	this->x = x;
	this->y = y;
	this->isMiddlePressed = true;
}

void Input::onLeftMouseReleased(int x, int y)
{
	this->x = x;
	this->y = y;
	this->isLeftPressed = false;
}

void Input::onRightMouseReleased(int x, int y)
{
	/*this->x = x;
	this->y = y;*/
	this->isRightPressed = false;
}

void Input::onMiddleMouseReleased(int x, int y)
{
	this->x = x;
	this->y = y;
	this->isMiddlePressed = false;
}

void Input::onMouseRawMove(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Input::getMouseLocation(int & x, int & y)
{
	x = this->x;
	y = this->y;
}

bool Input::readKeyboard()
{
	if (GetKeyboardState(mKeyboardState))
	{
		return true;
	}
	return false;
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
