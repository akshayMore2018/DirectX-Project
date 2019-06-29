#include "D3D.h"
#pragma comment(lib, "d3d11.lib") 

D3D::D3D()
	:driverType_(D3D_DRIVER_TYPE_NULL),
	featureLevel_(D3D_FEATURE_LEVEL_11_0),
	d3dDevice_(0), d3dContext_(0), swapChain_(0),
	backBufferTarget_(0)
{
}

D3D::~D3D()
{
	releaseAll();
}

bool D3D::initialize(HWND hwnd, unsigned int width, unsigned int height, bool fullscreen)
{
	D3D_DRIVER_TYPE driverTypes[] =
	{
	D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
	};

	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);
	D3D_FEATURE_LEVEL featureLevels[] =
	{
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0
	};
	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);


	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = !fullscreen;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	unsigned int creationFlag = 0;
#ifdef _DEBUG
	creationFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT result;
	unsigned int driver = 0;

	for (driver = 0; driver < totalDriverTypes; ++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
			creationFlag, featureLevels, totalFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, &swapChain_,
			&d3dDevice_, &featureLevel_, &d3dContext_);

		if (SUCCEEDED(result))
		{
			driverType_ = driverTypes[driver];
			break;
		}
	}
	if (FAILED(result))
	{
		MessageBox(hwnd, (LPCSTR)L"Direct3D device!", (LPCSTR)L"Failed to create the Direct3D device!", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
		return false;
	}

	ID3D11Texture2D* backBufferTexture;

	result = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	// __uuidof retrieves the GUID attached to the expression.
	//A GUID is a 128-bit value consisting of one group of 8 hexadecimal digits, 
	//followed by three groups of 4 hexadecimal digits each, 
	//followed by one group of 12 hexadecimal digits
	//example : 6B29FC40-CA47-1067-B31D-00DD010662DA
	//A GUID is a "Globally Unique ID"
	//GUIDs are also used to identify all interfaces and objects in COM programming. 


	if (FAILED(result))
	{
		MessageBox(hwnd, (LPCSTR)L"Swap Chain!", (LPCSTR)L"Failed to get the swap chain back buffer!", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
		return false;
	}

	result = d3dDevice_->CreateRenderTargetView(backBufferTexture, 0,
		&backBufferTarget_);
	if (backBufferTexture)
		backBufferTexture->Release();
	if (FAILED(result))
	{
		MessageBox(hwnd, (LPCSTR)L"Render target!", (LPCSTR)L"Failed to create the render target view!", MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2);
		return false;
	}
	d3dContext_->OMSetRenderTargets(1, &backBufferTarget_, 0);
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	d3dContext_->RSSetViewports(1, &viewport);
	

	return true;
}

void D3D::releaseAll()
{
	if (backBufferTarget_) backBufferTarget_->Release();
	if (swapChain_) swapChain_->Release();
	if (d3dContext_) d3dContext_->Release();
	if (d3dDevice_) d3dDevice_->Release();
	d3dDevice_ = 0;
	d3dContext_ = 0;
	swapChain_ = 0;
	backBufferTarget_ = 0;
}

void D3D::begin(float r, float g, float b, float a)
{
	float clearColor[4] = {r,g,b,a};
	d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);
}

void D3D::end()
{
	swapChain_->Present(0, 0);
}
