#include "BlankDemo.h"
#include <DirectXMath.h>

struct VertexPos
{
	DirectX::XMFLOAT3 pos;
};

BlankDemo::BlankDemo()
	:solidColorVS_(0), solidColorPS_(0),
	inputLayout_(0), vertexBuffer_(0)
{
}

BlankDemo::~BlankDemo()
{
	unLoadContent();
}

bool BlankDemo::loadContent()
{

	VertexPos vertices[]
	{
		DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f),
		DirectX::XMFLOAT3(0.5, -0.5f, 0.5f),
		DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f)

	};

	HRESULT d3dResult;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(VertexPos)*3;
	D3D11_SUBRESOURCE_DATA subResDesc = {};
	subResDesc.pSysMem = vertices;
	d3dResult = d3dDevice_->CreateBuffer(&bufferDesc, &subResDesc, &vertexBuffer_);
	if (FAILED(d3dResult))
	{
		return false;
	}


	ID3DBlob* vsBuffer=0;

	HRESULT result = D3DReadFileToBlob(L"VertexShader.cso", &vsBuffer);
	if (FAILED(result))
	{
		MessageBox(0, "Error Reading VertexShader.cso!", "Read Error", MB_OK);
		return false;
	}

	d3dResult=d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), nullptr, &solidColorVS_);

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
	d3dResult = d3dDevice_->CreateInputLayout(solidColorLayout,
		totalLayoutElements, vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(), &inputLayout_);
	vsBuffer->Release();
	
	if (FAILED(d3dResult))
	{
		return false;
	}


	ID3DBlob* psBuffer = 0;
	result = D3DReadFileToBlob(L"PixelShader.cso", &psBuffer);
	if (FAILED(result))
	{
		MessageBox(0, "Error Reading PixelShader.cso!", "Read Error", MB_OK);
		return false;
	}

	d3dResult = d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), nullptr, &solidColorPS_);

	if (FAILED(d3dResult))
	{
		if (psBuffer)
			psBuffer->Release();
		return false;
	}
	psBuffer->Release();
	



	return true;
}

void BlankDemo::unLoadContent()
{
	if (solidColorVS_) solidColorVS_->Release();
	if (solidColorPS_) solidColorPS_->Release();
	if (inputLayout_) inputLayout_->Release();
	if (vertexBuffer_) vertexBuffer_->Release();
	solidColorVS_ = 0;
	solidColorPS_ = 0;
	inputLayout_ = 0;
	vertexBuffer_ = 0;
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
	
	
	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;

	d3dContext_->IASetInputLayout(inputLayout_);
	d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	d3dContext_->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dContext_->VSSetShader(solidColorVS_, 0, 0);
	d3dContext_->PSSetShader(solidColorPS_, 0, 0);
	d3dContext_->Draw(3, 0);
	
	swapChain_->Present(0, 0);
}
