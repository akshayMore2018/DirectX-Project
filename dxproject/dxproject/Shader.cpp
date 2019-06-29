#include "Shader.h"
#include <d3dcompiler.h>
Shader::Shader()
	:solidColorVS_(0), solidColorPS_(0),
	inputLayout_(0), vertexBuffer_(0)
{
}

Shader::~Shader()
{
	if (solidColorVS_) solidColorVS_->Release();
	if (solidColorPS_) solidColorPS_->Release();
	if (inputLayout_) inputLayout_->Release();
	solidColorVS_ = 0;
	solidColorPS_ = 0;
	inputLayout_ = 0;
}

bool Shader::initialize(ID3D11Device * device, HWND hwnd)
{
	bool result = initializeShaders(device, hwnd, L"VertexShader.cso", L"PixelShader.cso");

	if (!result)
	{
		return false;
	}
	return true;
}


bool Shader::initializeShaders(ID3D11Device *device, HWND hwnd,  LPCWSTR vert, LPCWSTR frag)
{
	ID3DBlob* vsBuffer = 0;
	HRESULT d3dResult;
	HRESULT result = D3DReadFileToBlob(vert, &vsBuffer);
	if (FAILED(result))
	{
		MessageBox(0, "Error Reading VertexShader.cso!", "Read Error", MB_OK);
		return false;
	}

	d3dResult = device->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), nullptr, &solidColorVS_);

	if (FAILED(d3dResult))
	{
		if (vsBuffer)
			vsBuffer->Release();
		return false;
	}

	//input data layout

	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);
	d3dResult = device->CreateInputLayout(solidColorLayout,
		totalLayoutElements, vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(), &inputLayout_);
	vsBuffer->Release();

	if (FAILED(d3dResult))
	{
		return false;
	}


	ID3DBlob* psBuffer = 0;
	result = D3DReadFileToBlob(frag, &psBuffer);
	if (FAILED(result))
	{
		MessageBox(0, "Error Reading PixelShader.cso!", "Read Error", MB_OK);
		return false;
	}

	d3dResult = device->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), nullptr, &solidColorPS_);

	if (FAILED(d3dResult))
	{
		if (psBuffer)
			psBuffer->Release();
		return false;
	}
	psBuffer->Release();

	return true;
}


void Shader::render(ID3D11DeviceContext * deviceContext, int n)
{
	deviceContext->IASetInputLayout(inputLayout_);
	deviceContext->VSSetShader(solidColorVS_, 0, 0);
	deviceContext->PSSetShader(solidColorPS_, 0, 0);
	deviceContext->DrawIndexed(n, 0, 0);
}
