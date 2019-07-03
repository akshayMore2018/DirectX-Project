#include <Windows.h>
#include "Constants.h"
#include <memory>

#include "Engine.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	LPWSTR cmdLine, int cmdShow)
{
	Engine engine;

	bool result = engine.initialize();
	if (result)
	{
		while (engine.running())
		{
			engine.update();
			engine.render();
		}
	}

	return 0;
}