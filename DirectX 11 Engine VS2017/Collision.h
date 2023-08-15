#pragma once

#include <map>
#include <DirectXCollision.h>
using namespace DirectX;

enum CollsionType {
	obstacle,
	trigger,
	player
};

class Collision
{
public:

	XMVECTOR colliderAdjustTransform = XMVectorZero();
	XMVECTOR colliderAdjustScale = XMVectorSet(1.0f, 1.0f, 1.0f, 0);

	//BoundingOrientedBox
	BoundingOrientedBox obb;
	DirectX::XMFLOAT3* corners = NULL;
	void CreateObbByPointppp(int vcount, const char* vertex);

	BoundingSphere bsphere;
	void CreateSphereByPoint(int vcount, const char* vertex);

	CollsionType collisiontype;
private:

};

