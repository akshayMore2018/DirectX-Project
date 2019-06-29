#include "BlankDemo.h"
#include <DirectXMath.h>
#include "D3D.h"
#include "Model.h"
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
	triangle = new Model();
	bool success = triangle->initialize(mD3D->GetDevice());
	if (!success)
	{
		return false;
	}

	ID3DBlob* vsBuffer=0;
	HRESULT d3dResult;
	HRESULT result = D3DReadFileToBlob(L"VertexShader.cso", &vsBuffer);
	if (FAILED(result))
	{
		MessageBox(0, "Error Reading VertexShader.cso!", "Read Error", MB_OK);
		return false;
	}

	d3dResult= mD3D->GetDevice()->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), nullptr, &solidColorVS_);

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
	d3dResult = mD3D->GetDevice()->CreateInputLayout(solidColorLayout,
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

	d3dResult = mD3D->GetDevice()->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), nullptr, &solidColorPS_);

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
	solidColorVS_ = 0;
	solidColorPS_ = 0;
	inputLayout_ = 0;

	if (triangle)
		delete triangle;
	triangle = 0;
}

void BlankDemo::update(float dt)
{
}

void BlankDemo::render()
{
	if (mD3D->GetDeviceContext() == 0)
		return;
	
	mD3D->begin(0.0f, 0.0f, 0.25f, 1.0f);
	
	mD3D->GetDeviceContext()->IASetInputLayout(inputLayout_);

	triangle->render(mD3D->GetDeviceContext());

	mD3D->GetDeviceContext()->VSSetShader(solidColorVS_, 0, 0);
	mD3D->GetDeviceContext()->PSSetShader(solidColorPS_, 0, 0);
	mD3D->GetDeviceContext()->Draw(triangle->getIndexCount(), 0);
	
	mD3D->end();
}
