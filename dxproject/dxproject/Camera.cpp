#include "Camera.h"

Camera::Camera()
{
	this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->pos);
	this->rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rot);
	updateViewMatrix();
}

void Camera::setProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f)*XM_2PI;
	this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX & Camera::getViewMatrix() const
{
	return this->viewMatrix;
}

const XMMATRIX & Camera::getProjectionMatrix() const
{
	return this->projectionMatrix;
}

const XMVECTOR & Camera::getPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3 & Camera::getPositionFloat3() const
{
	return this->pos;
}

const XMVECTOR & Camera::getRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3 & Camera::getRotationFloat3() const
{
	return this->rot;
}

void Camera::setPosition(const XMVECTOR & pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->updateViewMatrix();
}

void Camera::setPosition(float x, float y, float z)
{
	this->pos = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->pos);
	this->updateViewMatrix();
}

void Camera::adjustPosition(const XMVECTOR & pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->pos, this->posVector);
	this->updateViewMatrix();
}

void Camera::adjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = XMLoadFloat3(&this->pos);
	this->updateViewMatrix();
}

void Camera::setRotation(const XMVECTOR & rot)
{
	XMStoreFloat3(&this->rot, rot);
	this->rotVector = rot;
	this->updateViewMatrix();
}

void Camera::setRotation(float x, float y, float z)
{
	this->rot = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->updateViewMatrix();
}

void Camera::adjustRotation(const XMVECTOR & rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->updateViewMatrix();
}

void Camera::adjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->updateViewMatrix();
}

void Camera::updateViewMatrix()
{
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
	XMVECTOR target = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, rotationMatrix);
	target += this->posVector;
	XMVECTOR up = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, rotationMatrix);
	this->viewMatrix = XMMatrixLookAtLH(this->posVector, target, up);
}
