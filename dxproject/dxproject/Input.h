#pragma once

#include<Windows.h>

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_Z 0x5A

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

	void onLeftMousePressed(int x, int y);
	void onRightMousePressed(int x, int y);
	void onMiddleMousePressed(int x, int y);
	
	void onLeftMouseReleased(int x, int y);
	void onRightMouseReleased(int x, int y);
	void onMiddleMouseReleased(int x, int y);

	void onMouseRawMove(int x, int y);
	bool isLeftPressed;
	bool isRightPressed;
	bool isMiddlePressed;
	void getMouseLocation(int & x, int &y);

private:
	bool readKeyboard();
	void processInput();

	unsigned char mKeyboardState[256];
	unsigned char keys_[VK_OEM_CLEAR + 1];
	int mScreenWidth, mScreenHeight;
	int x, y;
};