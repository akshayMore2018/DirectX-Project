#include "Shader.h"
#include <d3dcompiler.h>
Shader::Shader()
	:solidColorVS_(0), solidColorPS_(0),
	inputLayout_(0), vertexBuffer_(0),rasterState_(0), samplerState_(0)
{
}

Shader::~Shader()
{
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
		{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"Texcoord",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}

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


	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

	d3dResult = device->CreateRasterizerState(&rasterDesc, &rasterState_);
	if (FAILED(d3dResult))
	{
		MessageBox(0, "Error Creating Raster State!", "Raster Error", MB_OK);
		return false;
	}



	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	d3dResult = device->CreateSamplerState(&samplerDesc, &samplerState_);
	if (FAILED(d3dResult))
	{
		MessageBox(0, "Error Creating Sampler State!", "Sampler Error", MB_OK);

		return false;
	}

	return true;
}


void Shader::render(ID3D11DeviceContext * deviceContext, int n)
{
	deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(inputLayout_);
	deviceContext->RSSetState(rasterState_);
	deviceContext->PSSetSamplers(0, 1, &samplerState_);
	deviceContext->VSSetShader(solidColorVS_, 0, 0);
	deviceContext->PSSetShader(solidColorPS_, 0, 0);
	deviceContext->DrawIndexed(n, 0, 0);
}
