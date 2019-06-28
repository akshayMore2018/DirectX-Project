#include "BlankDemo.h"

BlankDemo::BlankDemo()
{
}

BlankDemo::~BlankDemo()
{
}

bool BlankDemo::loadContent()
{
	return true;
}

void BlankDemo::unLoadContent()
{
}

void BlankDemo::update(float dt)
{
}

void BlankDemo::render()
{
	if (d3dContext_ == 0)
		return;

	float clearColor[4] = { 0.0f,0.0f,0.25f,1.0f };
	d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);
	swapChain_->Present(0, 0);
}
