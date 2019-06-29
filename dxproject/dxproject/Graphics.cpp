#include "Graphics.h"
#include <memory>
#include "D3D.h"
#include "Model.h"
#include "Shader.h"

#pragma comment(lib, "D3DCompiler.lib") 

Graphics::Graphics():mD3D(0),triangle(0),shader(0),
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
}

bool Graphics::loadContent()
{

	triangle = new Model();
	bool success = triangle->initialize(mD3D->GetDevice());
	if (!success)
	{
		return false;
	}

	shader = new Shader();
	success = shader->initialize(mD3D->GetDevice(), hwnd);

	if (!success)
	{
		return false;
	}
	return true;
}

void Graphics::unLoadContent()
{
	if (shader)
		delete shader;
	shader = 0;

	if (triangle)
		delete triangle;
	triangle = 0;

	if (mD3D)
		delete mD3D;
	mD3D = 0;
}

void Graphics::update(float dt)
{
}

void Graphics::render()
{
	if (mD3D->GetDeviceContext() == 0)
		return;

	mD3D->begin(0.0f, 0.0f, 0.25f, 1.0f);

	triangle->render(mD3D->GetDeviceContext());

	shader->render(mD3D->GetDeviceContext(), triangle->getIndexCount());

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

