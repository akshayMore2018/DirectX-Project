#pragma once
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <DirectXMath.h>
#include "ConstantBuffer.h"

class Graphics
{
public:

	struct VertexPos
	{
		VertexPos() {}
		VertexPos(float x, float y, float z, float u, float v)
			: pos(x, y, z), texCoord(u, v) {}

		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 texCoord;
	};

	struct Matrix
	{
		DirectX::XMMATRIX mat;
	};


	Graphics();
	virtual ~Graphics();	
	HRESULT initialize(HWND hwnd, int width, int height);

	void update(float dt);
	void render();

private:
	HRESULT initializeDirectX(HWND hwnd);
	HRESULT initializeShaders();
	HRESULT initializeScene();
	int width;
	int height;
	HINSTANCE hInstance;
	HWND hwnd;
	class Camera* camera;
	
	D3D_DRIVER_TYPE driverType_;
	D3D_FEATURE_LEVEL featureLevel_;
	ID3D11Device* d3dDevice_;//The Direct3D device is the device itself and communicates with the hardware.
	//The Direct3D context is a rendering context that tells the device how to draw.
	//It also includes rendering states and other drawing information
	ID3D11DeviceContext* d3dContext_;

	IDXGISwapChain* swapChain_; //collection of rendering destination for a device and context to draw to.
	ID3D11RenderTargetView* backBufferTarget_;

	ID3D11Texture2D* depthStencilBuffer_;
	ID3D11DepthStencilView* depthStencilView_;
	ID3D11DepthStencilState* depthStencilState_;

	ID3D11RasterizerState* rasterState_;
	ID3D11SamplerState* samplerState_;


	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> mSpriteFont;

	ID3D11VertexShader* solidColorVS_;
	ID3D11PixelShader* solidColorPS_;
	ID3D11InputLayout* inputLayout_;
	ID3D11Buffer* vertexBuffer_;

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount, mIndexCount;
	ID3D11ShaderResourceView* mTex;
	ConstantBuffer<Matrix> mConstantBuffer;

};