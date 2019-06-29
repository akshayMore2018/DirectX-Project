#include <Windows.h>
#include "Constants.h"
#include <memory>

#include "System.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	LPWSTR cmdLine, int cmdShow)
{
	System* system;
	

	system = new System();

	bool result = system->initialize();
	if (result)
	{
		system->run();
	}

	system->release();
	delete system;
	system = 0;

	return 0;
}