#pragma once

#include "Graphics.h"

class BlankDemo : public Graphics
{
public:
	BlankDemo();
	virtual ~BlankDemo();

	bool loadContent();
	void unLoadContent();

	void update(float dt);
	void render();

private:
	ID3D11VertexShader* solidColorVS_;
	ID3D11PixelShader* solidColorPS_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;
};