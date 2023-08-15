#pragma once
#include <DirectXCollision.h>
using namespace DirectX;

#define EPA_TOLERANCE 0.0001
#define EPA_MAX_NUM_FACES 64
#define EPA_MAX_NUM_LOOSE_EDGES 32
#define EPA_MAX_NUM_ITERATIONS 64
class GJKCollision
{
public:
	bool gjk(const BoundingOrientedBox& obb,
		const BoundingSphere& sphere, DirectX::XMVECTOR& mtv);

	void update_simplex3(XMVECTOR& a, XMVECTOR& b, XMVECTOR& c, XMVECTOR& d, int& simp_dim, XMVECTOR& search_dir);
	bool update_simplex4(XMVECTOR& a, XMVECTOR& b, XMVECTOR& c, XMVECTOR& d, int& simp_dim, XMVECTOR& search_dir);

	XMVECTOR EPA(XMVECTOR& a, XMVECTOR& b, XMVECTOR& c, XMVECTOR& d, const BoundingOrientedBox& obb, const BoundingSphere& sphere);

	XMVECTOR GetSupportOBB(const XMVECTOR& dir, DirectX::XMFLOAT3* corners);
	XMVECTOR GetSupportCirecle(const XMVECTOR& dir, const BoundingSphere& sphere);
	XMVECTOR GetSupportPoints(const XMVECTOR& dir, DirectX::XMFLOAT3* corners, int number);

	float GetMaxX(DirectX::XMFLOAT3* corners);
	float GetMinX(DirectX::XMFLOAT3* corners);
	float GetMaxY(DirectX::XMFLOAT3* corners);
	float GetMinY(DirectX::XMFLOAT3* corners);
	float GetMaxZ(DirectX::XMFLOAT3* corners);
	float GetMinZ(DirectX::XMFLOAT3* corners);
};



