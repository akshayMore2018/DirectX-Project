#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class Camera
{
public:
	Camera();
	void setProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ); 
	
	const XMMATRIX & getViewMatrix() const;
	const XMMATRIX & getProjectionMatrix() const;

	const XMVECTOR & getPositionVector() const;
	const XMFLOAT3 & getPositionFloat3() const;
	const XMVECTOR & getRotationVector() const;
	const XMFLOAT3 & getRotationFloat3() const;

	void setPosition(const XMVECTOR& pos);
	void setPosition(float x, float y, float z);
	void adjustPosition(const XMVECTOR& pos);
	void adjustPosition(float x, float y, float z);
	void setRotation(const XMVECTOR& rot);
	void setRotation(float x, float y, float z);
	void adjustRotation(const XMVECTOR& rot);
	void adjustRotation(float x, float y, float z);

private:
	void updateViewMatrix();
	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);




};