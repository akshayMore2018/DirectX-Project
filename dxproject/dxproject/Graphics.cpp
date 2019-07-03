#include "Graphics.h"
#include <memory>
#include "Camera.h"
#include <WICTextureLoader.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib") 
#pragma comment(lib, "DirectXTK.lib")

Graphics::Graphics()
	:driverType_(D3D_DRIVER_TYPE_NULL),
	featureLevel_(D3D_FEATURE_LEVEL_11_0),
	d3dDevice_(0), d3dContext_(0), swapChain_(0),
	backBufferTarget_(0), depthStencilView_(0),
	depthStencilBuffer_(0), depthStencilState_(0),
	solidColorVS_(0), solidColorPS_(0),
	inputLayout_(0), vertexBuffer_(0), rasterState_(0), samplerState_(0),
	mVertexBuffer(0), mIndexBuffer(0),
	mVertexCount(0), mIndexCount(0), mTex(0),
	width(0),height(0),camera(0)
{

}

Graphics::~Graphics()
{
	if (camera)
		delete camera;
	camera = 0;

	if (mTex)mTex->Release();
	mTex = 0;

	if (mVertexBuffer) mVertexBuffer->Release();
	if (mIndexBuffer) mIndexBuffer->Release();

	mVertexBuffer = 0;
	mIndexBuffer = 0;

	if (rasterState_)rasterState_->Release();
	if (samplerState_)samplerState_->Release();
	if (solidColorVS_) solidColorVS_->Release();
	if (solidColorPS_) solidColorPS_->Release();
	if (inputLayout_) inputLayout_->Release();
	rasterState_ = 0;
	samplerState_ = 0;
	solidColorVS_ = 0;
	solidColorPS_ = 0;
	inputLayout_ = 0;

	if (depthStencilState_)depthStencilState_->Release();
	if (depthStencilBuffer_)depthStencilBuffer_->Release();
	if (depthStencilView_)depthStencilView_->Release();
	if (backBufferTarget_) backBufferTarget_->Release();
	if (swapChain_) swapChain_->Release();
	if (d3dContext_) d3dContext_->Release();
	if (d3dDevice_) d3dDevice_->Release();
	d3dDevice_ = 0;
	d3dContext_ = 0;
	swapChain_ = 0;
	backBufferTarget_ = 0;
	depthStencilView_ = 0;
	depthStencilBuffer_ = 0;
	depthStencilState_ = 0;


}

void Graphics::update(float dt)
{

}

void Graphics::render()
{
	if (d3dContext_ == 0)
		return;
	float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
	d3dContext_->ClearRenderTargetView(backBufferTarget_, clearColor);
	d3dContext_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	d3dContext_->IASetInputLayout(inputLayout_);
	d3dContext_->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dContext_->RSSetState(rasterState_);
	d3dContext_->OMSetDepthStencilState(depthStencilState_, 0);
	d3dContext_->PSSetSamplers(0, 1, &samplerState_);
	d3dContext_->VSSetShader(solidColorVS_, 0, 0);
	d3dContext_->PSSetShader(solidColorPS_, 0, 0);
	
	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;

	XMMATRIX world = XMMatrixIdentity();
	camera->adjustPosition(0.0f, 0.0f, 0.01f);
	mConstantBuffer.data.mat = world * camera->getViewMatrix() * camera->getProjectionMatrix();
	mConstantBuffer.data.mat = XMMatrixTranspose(mConstantBuffer.data.mat);

	if (!mConstantBuffer.applyChanges(d3dContext_))
	{
		return;
	}
	d3dContext_->VSSetConstantBuffers(0, 1, mConstantBuffer.get());
	d3dContext_->PSSetShaderResources(0, 1, &mTex);
	d3dContext_->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	d3dContext_->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3dContext_->DrawIndexed(mIndexCount, 0, 0);

	mSpriteBatch->Begin();

	mSpriteFont->DrawString(mSpriteBatch.get(), L"DirectX11 Demo", DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(1.0, 1.0));

	mSpriteBatch->End();

	swapChain_->Present(1, NULL);
}

HRESULT Graphics::initialize(HWND hwnd, int width, int height)
{
	this->width = width;
	this->height = height;
	HRESULT hr = initializeDirectX(hwnd);

	if (SUCCEEDED(hr))
	{
		hr = initializeShaders();
		if (SUCCEEDED(hr))
		{
			hr = initializeScene();
		}
	}
	return hr;
}

HRESULT Graphics::initializeDirectX(HWND hwnd)
{
	HRESULT result = S_OK;

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
	swapChainDesc.BufferDesc.Width = this->width;
	swapChainDesc.BufferDesc.Height = this->height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	unsigned int creationFlag = 0;
#ifdef _DEBUG
	creationFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

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
		MessageBox(hwnd, (LPCSTR)L"Direct3D device!", (LPCSTR)L"Failed to create the device and swap chain", MB_OK);
		return result;
	}

	ID3D11Texture2D* backBufferTexture;

	result = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferTexture));
	// __uuidof retrieves the GUID attached to the expression.
	//A GUID is a 128-bit value consisting of one group of 8 hexadecimal digits, 
	//followed by three groups of 4 hexadecimal digits each, 
	//followed by one group of 12 hexadecimal digits
	//example : 6B29FC40-CA47-1067-B31D-00DD010662DA
	//A GUID is a "Globally Unique ID"
	//GUIDs are also used to identify all interfaces and objects in COM programming. 


	if (FAILED(result))
	{
		MessageBox(hwnd, (LPCSTR)L"Back Buffer!", (LPCSTR)L"Failed to get back buffer!", MB_OK);
		return result;
	}

	result = d3dDevice_->CreateRenderTargetView(backBufferTexture, 0,
		&backBufferTarget_);
	if (backBufferTexture)
		backBufferTexture->Release();
	
	if (FAILED(result))
	{
		MessageBox(hwnd, (LPCSTR)L"Render target!", (LPCSTR)L"Failed to create the render target view!", MB_OK);
		return result;
	}

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	depthStencilBufferDesc.Width = this->width;
	depthStencilBufferDesc.Height = this->height;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	result = d3dDevice_->CreateTexture2D(&depthStencilBufferDesc, NULL, &depthStencilBuffer_);
	if (FAILED(result))
	{
		MessageBox(0, "Error Creating Depth Stencil Buffer!", "Buffer Error", MB_OK);
		return result;
	}

	result = d3dDevice_->CreateDepthStencilView(depthStencilBuffer_, NULL, &depthStencilView_);
	if (FAILED(result))
	{
		MessageBox(0, "Error Creating Depth Stencil View!", "View Error", MB_OK);
		return result;
	}

	d3dContext_->OMSetRenderTargets(1, &backBufferTarget_, depthStencilView_);

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	result = d3dDevice_->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState_);
	if (FAILED(result))
	{
		MessageBox(0, "Error Creating Depth Stencil State!", "View Error", MB_OK);
		return result;
	}

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	
	d3dContext_->RSSetViewports(1, &viewport);


	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	result = d3dDevice_->CreateRasterizerState(&rasterDesc, &rasterState_);
	if (FAILED(result))
	{
		MessageBox(0, "Error Creating Raster State!", "Raster Error", MB_OK);
		return result;
	}

	mSpriteBatch = std::make_unique<DirectX::SpriteBatch>(d3dContext_);
	mSpriteFont = std::make_unique<DirectX::SpriteFont>(d3dDevice_, L"Assets\\Fonts\\comic_sans_ms_16.spritefont");

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = d3dDevice_->CreateSamplerState(&samplerDesc, &samplerState_);
	if (FAILED(result))
	{
		MessageBox(0, "Error Creating Sampler State!", "Sampler Error", MB_OK);
	}

	return result;
}

HRESULT Graphics::initializeShaders()
{
	HRESULT result = S_OK;

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"Texcoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}

	};

	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);

	ID3DBlob* vsBuffer = 0;
	result = D3DReadFileToBlob(L"VertexShader.cso", &vsBuffer);
	if (FAILED(result))
	{
		MessageBox(0, "Error Reading VertexShader.cso!", "Read Error", MB_OK);
		return result;
	}

	result = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), nullptr, &solidColorVS_);
	if (FAILED(result))
	{
		if (vsBuffer)
			vsBuffer->Release();
		MessageBox(0, "Error Creating VertexShader!", "Create Error", MB_OK);
		return result;
	}
	result = d3dDevice_->CreateInputLayout(solidColorLayout,
		totalLayoutElements, vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(), &inputLayout_);
	vsBuffer->Release();

	if (FAILED(result))
	{
		MessageBox(0, "Error Creating InputLayout!", "Layout Error", MB_OK);
		return result;
	}

	ID3DBlob* psBuffer = 0;
	result = D3DReadFileToBlob(L"PixelShader.cso", &psBuffer);
	if (FAILED(result))
	{
		MessageBox(0, "Error Reading PixelShader.cso!", "Read Error", MB_OK);
		return result;
	}

	result = d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), nullptr, &solidColorPS_);

	psBuffer->Release();

	return result;
}

HRESULT Graphics::initializeScene()
{
	HRESULT result = S_OK;

	VertexPos vertices[] =
	{
		//position-------------------------------------color
		VertexPos(-0.5f,-0.5f, 0.0f,0.0f, 1.0f),
		VertexPos(-0.5f, 0.5f, 0.0f,0.0f, 0.0f),
		VertexPos( 0.5f, 0.5f, 0.0f,1.0f, 0.0f),
		VertexPos( 0.5f,-0.5f, 0.0f,1.0f, 1.0f)
	};
	mVertexCount = sizeof(vertices) / sizeof(VertexPos);

	unsigned long indices[] =
	{
		0,1,2,
		0,2,3
	};
	mIndexCount = sizeof(indices) / sizeof(unsigned long);



	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = sizeof(VertexPos);

	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = d3dDevice_->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(result))
	{
		MessageBox(0, "Error Reading Vertex buffer!", "Buffer Error", MB_OK);
		return result;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = sizeof(unsigned long);

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = d3dDevice_->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);

	if (FAILED(result))
	{
		MessageBox(0, "Error Reading Index buffer!", "Buffer Error", MB_OK);
		return result;
	}

	result = DirectX::CreateWICTextureFromFile(d3dDevice_, L"Assets\\Textures\\brick.png", nullptr, &mTex);
	if (FAILED(result))
	{
		MessageBox(0, "Error Creating texture!", "texture Error", MB_OK);

		return result;
	}


	result = this->mConstantBuffer.initialize(d3dDevice_);

	if (FAILED(result))
	{
		MessageBox(0, "Error Creating const buffer!", "Buffer Error", MB_OK);
	}

	camera = new Camera();
	camera->setPosition(0.0f, 0.0f, -2.0f);
	camera->setProjectionValues(90.0f, static_cast<float>(this->width) / static_cast<float>(this->height), 0.1f, 1000.0f);


	return result;
}




