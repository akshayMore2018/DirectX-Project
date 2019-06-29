#pragma once
#include <d3dcompiler.h>

class Graphics
{
public:
	Graphics();
	virtual ~Graphics();
	
	bool initialize(HINSTANCE hInstance, HWND hw, bool fullscreen);
	void releaseAll();
	virtual bool loadContent();
	virtual void unLoadContent();

	virtual void update(float dt);
	virtual void render();

private:
	bool fullscreen;
	unsigned int width;
	unsigned int height;
	
protected:
	HINSTANCE hInstance;
	HWND hwnd;
	class D3D* mD3D;
	class Model* triangle;
	
};