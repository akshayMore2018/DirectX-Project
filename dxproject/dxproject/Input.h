#pragma once
#define DIRECTINPUT_VERSION 0x0800


#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_Z 0x5A

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class Input
{
public:
	enum KeyState
	{
		KEY_STATE_NONE = 0,
		KEY_STATE_PRESSED = 1 << 0,
		KEY_STATE_HELD = 1 << 1,
		KEY_STATE_RELEASED = 1 << 2,
	};
	Input();
	~Input();

	HRESULT initialize(HWND, int, int);
	void update();

	bool isKeyPressed(int virtualKey) const;
	bool isKeyHeld(int virtualKey) const;
	bool isKeyReleased(int virtualKey) const;
	unsigned char getKeyState(int virtualKey) const;
private:
	bool readKeyboard();
	bool readMouse();
	void processInput();

	IDirectInput8* mInput;
	IDirectInputDevice8* mMouse;
	unsigned char mKeyboardState[256];
	unsigned char keys_[VK_OEM_CLEAR + 1];
	int mScreenWidth, mScreenHeight;
	int mMouseX, mMouseY;
	DIMOUSESTATE mMouseState;

};