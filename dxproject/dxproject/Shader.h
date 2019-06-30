#pragma once
#include <d3d11.h>

class Shader
{
public:
	Shader();
	~Shader();

	bool initialize(ID3D11Device* device, HWND hwnd);
	
	void render(ID3D11DeviceContext* deviceContext, int n);
private:
	bool initializeShaders(ID3D11Device*, HWND hwnd, LPCWSTR vert, LPCWSTR frag);
	ID3D11VertexShader* solidColorVS_;
	ID3D11PixelShader* solidColorPS_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;

	ID3D11RasterizerState* rasterState_;
};