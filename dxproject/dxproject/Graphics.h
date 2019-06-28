#pragma once
#pragma comment(lib, "d3d11.lib") 
#include <d3d11.h>


class Graphics
{
public:
	Graphics();
	virtual ~Graphics();
	void releaseAll();
	bool initialize(HWND hw,bool fullscreen);
	HRESULT showBackBuffer();


private:
	HWND hwnd;
	D3D_DRIVER_TYPE driverType_;
	D3D_FEATURE_LEVEL featureLevel_;
	ID3D11Device* d3dDevice_;//The Direct3D device is the device itself and communicates with the hardware.
	//The Direct3D context is a rendering context that tells the device how to draw.
	//It also includes rendering states and other drawing information
	ID3D11DeviceContext* d3dContext_;

	IDXGISwapChain* swapChain_; //collection of rendering destination for a device and context to draw to.
	ID3D11RenderTargetView* backBufferTarget_;

	bool fullscreen;
	unsigned int width;
	unsigned int height;

	void initD3Dpp();
};



//DEVICE TYPES:
//HARDWARE (runs on the graphics hardware)
//SOFTWARE (allows devs to write their own software rendering driver and use it with Direct3D.
//WARP (efficient CPU-rendering device)
//REFERENCE (emulates hardware rendering on cpu)