#pragma once
#include <d3d11.h>
class D3D
{
public:
	D3D();
	~D3D();

	bool initialize(HWND hw,unsigned int width,unsigned int height, bool fullscreen);
	void releaseAll();
	void begin(float r, float g, float b, float a);
	void end();

	ID3D11Device* GetDevice() { return d3dDevice_; };
	ID3D11DeviceContext* GetDeviceContext() { return d3dContext_; };

private:
	D3D_DRIVER_TYPE driverType_;
	D3D_FEATURE_LEVEL featureLevel_;
	ID3D11Device* d3dDevice_;//The Direct3D device is the device itself and communicates with the hardware.
	//The Direct3D context is a rendering context that tells the device how to draw.
	//It also includes rendering states and other drawing information
	ID3D11DeviceContext* d3dContext_;

	IDXGISwapChain* swapChain_; //collection of rendering destination for a device and context to draw to.
	ID3D11RenderTargetView* backBufferTarget_;

	//D3D_DRIVER_TYPE driverType_;
	//D3D_FEATURE_LEVEL featureLevel_;
	//ID3D11Device* d3dDevice_;//The Direct3D device is the device itself and communicates with the hardware.
	////The Direct3D context is a rendering context that tells the device how to draw.
	////It also includes rendering states and other drawing information
	//ID3D11DeviceContext* d3dContext_;

	//IDXGISwapChain* swapChain_; //collection of rendering destination for a device and context to draw to.
	//ID3D11RenderTargetView* backBufferTarget_;
};

//DEVICE TYPES:
//HARDWARE (runs on the graphics hardware)
//SOFTWARE (allows devs to write their own software rendering driver and use it with Direct3D.
//WARP (efficient CPU-rendering device)
//REFERENCE (emulates hardware rendering on cpu)