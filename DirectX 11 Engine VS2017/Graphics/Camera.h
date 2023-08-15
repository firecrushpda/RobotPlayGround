#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX & GetViewMatrix() const;
	const XMMATRIX & GetProjectionMatrix() const;

	const XMVECTOR & GetPositionVector() const;
	const XMFLOAT3 & GetPositionFloat3() const;
	const XMVECTOR & GetRotationVector() const;
	const XMFLOAT3 & GetRotationFloat3() const;

	const XMFLOAT3 & GetFocusPositionFloat3()const;
	const XMVECTOR & GetFocusPositionVector() const;
	void SetFocusPosition(const XMVECTOR & pos);
	void SetFocusPosition(float x, float y, float z);

	void SetPosition(const XMVECTOR & pos);
	void SetPosition(float x, float y, float z);
	void AdjustPosition(const XMVECTOR & pos);
	void AdjustPosition(float x, float y, float z);
	void SetRotation(const XMVECTOR & rot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR & rot);
	void AdjustRotation(float x, float y, float z);
	void SetLookAtPos(XMFLOAT3 lookAtPos);
	const XMVECTOR & GetForwardVector();
	const XMVECTOR & GetRightVector();
	const XMVECTOR & GetBackwardVector();
	const XMVECTOR & GetLeftVector();
	const XMVECTOR & GetUpVector();
	const XMVECTOR & GetDownVector();

	void SetCameraType(int type);
	int GetCameraType();
	void SetCameraFocusPoint(XMFLOAT3 fpoint);
	
private:
	void UpdateViewMatrix();
	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	int cameratype = 0;//0 = free 1 = focus
	XMFLOAT3 fpos;
	XMVECTOR fposVector;
	XMFLOAT3 frot;
	XMVECTOR frotVector;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_DOWN_VECTOR = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward;
	XMVECTOR vec_left;
	XMVECTOR vec_right;
	XMVECTOR vec_backward;
	XMVECTOR vec_up;
	XMVECTOR vec_down;
};
