#pragma once
#include <DirectXMath.h>
#include <WICTextureLoader.h>
#include "ConstantBuffer.h"

class Model
{
public:

	struct VertexPos
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 texCoord;
	};

	struct Vector2
	{
		float x = 0;
		float y = 0;
	};

	Model();
	~Model();

	bool initialize(ID3D11Device* device);
	void release();
	void render(ID3D11DeviceContext* deviceContext);

	int getIndexCount();

private:
	bool initializeBuffers(ID3D11Device* device);
	void releaseBuffers();
	void renderBuffers(ID3D11DeviceContext* deviceContext);

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	
	ConstantBuffer<Vector2> mConstantBuffer;
	int mVertexCount, mIndexCount;

	ID3D11ShaderResourceView* mTex;
};