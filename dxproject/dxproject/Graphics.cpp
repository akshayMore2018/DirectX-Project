#include "Graphics.h"
#include <memory>
#include "D3D.h"
#include "Model.h"

#pragma comment(lib, "D3DCompiler.lib") 

Graphics::Graphics():mD3D(0),triangle(0),
	fullscreen(false),width(0),height(0)
{

}

Graphics::~Graphics()
{
	releaseAll();
}

void Graphics::releaseAll()
{
	unLoadContent();
	if(mD3D)
	delete mD3D;
	mD3D = 0;
}

bool Graphics::loadContent()
{
	return true;
}

void Graphics::unLoadContent()
{
}

void Graphics::update(float dt)
{
}

void Graphics::render()
{
	if (mD3D->GetDeviceContext() == 0)
		return;

	mD3D->begin(0.0f, 0.0f, 0.25f, 1.0f);



	mD3D->end();
}

bool Graphics::initialize(HINSTANCE hInstance, HWND hw, bool fullscreen)
{
	hwnd = hw;
	this->hInstance = hInstance;
	this->fullscreen = fullscreen;

	RECT dimensions;
	GetClientRect(hwnd, &dimensions);
	this->width = dimensions.right - dimensions.left;
	this->height = dimensions.bottom - dimensions.top;

	mD3D = new D3D();
	bool result = mD3D->initialize(hwnd,width,height,fullscreen);
	if (!result)
	{
		return false;
	}

	return loadContent();
}

