#pragma once
#include <d3d11.h>
template<class T>
class ConstantBuffer
{
public:

	ConstantBuffer():mBuffer(0) {};
	~ConstantBuffer() 
	{
		if (mBuffer)mBuffer->Release();
		mBuffer = 0;
	};
	HRESULT initialize(ID3D11Device* device)
	{

		D3D11_BUFFER_DESC constBufferDesc;
		constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constBufferDesc.MiscFlags = 0;
		constBufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - sizeof(T) % 16));
		constBufferDesc.StructureByteStride = 0;

		HRESULT result = device->CreateBuffer(&constBufferDesc, 0, &mBuffer);
		return result;


	}
	
	ID3D11Buffer* const* get() const { return &mBuffer; }


	bool applyChanges(ID3D11DeviceContext* deviceContext)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = deviceContext->Map(mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			MessageBox(0, "Error Mapping const buffer!", "Buffer Error", MB_OK);
			return false;
		}
		CopyMemory(mappedResource.pData, &data, sizeof(T));
		deviceContext->Unmap(mBuffer, 0);
		return true;
	}
	T data;
private:
	ID3D11Buffer* mBuffer = nullptr;
	
};



