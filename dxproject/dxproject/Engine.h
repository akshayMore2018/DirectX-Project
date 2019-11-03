#pragma once
#include <Windows.h>
#include <iostream>
class Engine
{
protected:
	//Reference to Graphics class
	class Graphics	* mGraphics;
	//Reference to Input class
	class Input		* mInput;
	//Windows handle
	HWND			Hwnd;
	//Standard return type
	HRESULT			Hr;

	bool			Initialized;

public:
	//constructor
	Engine();
	//Destructor
	virtual ~Engine();
	
public:
	//Initialize the Game
	virtual void Initialize(HWND Hwnd);

	virtual void Run(HWND Hwnd);

	virtual void RenderGame();

	virtual void ReleaseAll();

	virtual void ResetAll();

	virtual void DeleteAll();

	//Windows message handler
	LRESULT MessageHandler(HWND Hwnd, UINT Message, WPARAM WParam, LPARAM LParam);
	

	
public:

	Graphics	*GetGraphics();

	Input		*GetInput();

public:

	void ExitGame();
	void processInput();
};