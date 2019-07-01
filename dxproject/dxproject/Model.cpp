#include "Model.h"

Model::Model():
	mVertexBuffer(0),mIndexBuffer(0),
	mVertexCount(0),mIndexCount(0), mTex(0)
{
}

Model::~Model()
{
	release();
}

bool Model::initialize(ID3D11Device * device)
{
	bool success = initializeBuffers(device);
	if (!success)
	{
		return false;
	}

	HRESULT result = DirectX::CreateWICTextureFromFile(device, L"Assets\\Textures\\brick.png", nullptr, &mTex);
	if (FAILED(result))
	{
		MessageBox(0, "Error Creating texture!", "texture Error", MB_OK);

		return false;
	}

	return true;
}

void Model::release()
{
	if (mTex)mTex->Release();
	mTex = 0;
	releaseBuffers();
}

void Model::render(ID3D11DeviceContext * deviceContext)
{
	renderBuffers(deviceContext);
}

int Model::getIndexCount()
{
	return mIndexCount;
}

bool Model::initializeBuffers(ID3D11Device * device)
{
	VertexPos vertices[]=
	{
		//position-------------------------------------color
		{DirectX::XMFLOAT3(-0.5f,-0.5f, 0.5f),DirectX::XMFLOAT2(0.0f, 1.0f)},
		{DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f),DirectX::XMFLOAT2(0.0f, 0.0f)},
		{DirectX::XMFLOAT3( 0.5f, 0.5f, 0.5f),DirectX::XMFLOAT2(1.0f, 0.0f)},
		{DirectX::XMFLOAT3( 0.5f,-0.5f, 0.5f),DirectX::XMFLOAT2(1.0f, 1.0f)}
	};
	mVertexCount = sizeof(vertices)/sizeof(VertexPos);

	unsigned long indices[]=
	{
		0,1,2,
		0,2,3
	};
	mIndexCount = sizeof(indices)/sizeof(unsigned long);
	
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = sizeof(VertexPos);

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
	if (FAILED(result))
	{
		return false;
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

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);

	if (FAILED(result))
	{
		return false;
	}

	

	result = this->mConstantBuffer.initialize(device);

	if (FAILED(result))
	{
		MessageBox(0, "Error Creating const buffer!", "Buffer Error", MB_OK);
		return false;
	}



	return true;
}

void Model::releaseBuffers()
{
	if (mVertexBuffer) mVertexBuffer->Release();
	if (mIndexBuffer) mIndexBuffer->Release();

	mVertexBuffer = 0;
	mIndexBuffer = 0;

}

void Model::renderBuffers(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;

	mConstantBuffer.data.x = 0.0f;
	mConstantBuffer.data.y = 0.3f;

	if (!mConstantBuffer.applyChanges(deviceContext))
	{
		return;
	}
	deviceContext->VSSetConstantBuffers(0, 1, mConstantBuffer.get());
	deviceContext->PSSetShaderResources(0, 1, &mTex);
	deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIndexBuffer,DXGI_FORMAT_R32_UINT,0);
}
