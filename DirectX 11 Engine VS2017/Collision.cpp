#include "Collision.h"
#include "Graphics/Vertex.h"
#include <vector>

void Collision::CreateObbByPointppp(int vcount,const char* vertex)
{
	//create bounding box
	auto tmep =(Vertex*) (vertex);//(DirectX::XMFLOAT3*) 

	std::vector<DirectX::XMFLOAT3>list;
	for (size_t i = 0; i < vcount; i++)
	{
		list.push_back(XMFLOAT3(tmep[i].pos.x, tmep[i].pos.y, tmep[i].pos.z));
	}

	
	BoundingBox tempbb; tempbb.CreateFromPoints(tempbb, vcount, list.data(), sizeof(DirectX::XMFLOAT3));
	list.clear();
	corners = NULL;
	corners = (DirectX::XMFLOAT3*)malloc(sizeof(DirectX::XMFLOAT3) * 8u);
	tempbb.GetCorners((DirectX::XMFLOAT3*)corners);

	this->obb.CreateFromPoints(obb, 8u, corners, sizeof(DirectX::XMFLOAT3));
	obb.Transform(obb, XMMatrixScalingFromVector(colliderAdjustScale) * XMMatrixTranslationFromVector(colliderAdjustTransform));

	corners = NULL;
	corners = (DirectX::XMFLOAT3*)malloc(sizeof(DirectX::XMFLOAT3) * 8u);
	obb.GetCorners((DirectX::XMFLOAT3*)corners);
}

void Collision::CreateSphereByPoint(int vcount, const char* vertex)
{
	//create sphere
	auto tmep = (Vertex*)(vertex);
	std::vector<DirectX::XMFLOAT3>list;
	for (size_t i = 0; i < vcount; i++)
	{
		list.push_back(XMFLOAT3(tmep[i].pos.x, tmep[i].pos.y, tmep[i].pos.z));
	}

	this->bsphere.CreateFromPoints(bsphere, vcount, list.data(), sizeof(DirectX::XMFLOAT3));
	bsphere.Transform(bsphere, XMMatrixScalingFromVector(colliderAdjustScale) * XMMatrixTranslationFromVector(colliderAdjustTransform));
	list.clear();
	//we dont create point list here we create in Model initial
}

