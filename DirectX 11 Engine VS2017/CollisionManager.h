#pragma once
#define GravitySpeed	9.8f;

#include <DirectXCollision.h>
using namespace DirectX;

struct CollisionRay
{
	XMVECTOR origin;
	XMVECTOR direction;
};

class CollisionManager
{
public:
	//std::vector<BoundingBox> bblist;

	/*void AddBoundingBoxToList(BoundingBox& bb) {
		bblist.push_back(bblist);
	}*/
}; 

