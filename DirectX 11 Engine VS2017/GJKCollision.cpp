#include "GJKCollision.h"


bool GJKCollision::gjk(const BoundingOrientedBox& obb,const BoundingSphere& sphere, DirectX::XMVECTOR& mtv)
{
	DirectX::XMFLOAT3* corners = (DirectX::XMFLOAT3*)malloc(sizeof(DirectX::XMFLOAT3) * 8u);
	obb.GetCorners((DirectX::XMFLOAT3*)corners);

	XMVECTOR a, b, c, d;
	XMVECTOR search_dir = XMLoadFloat3(&obb.Center) - XMLoadFloat3(&sphere.Center);//temp

	//support of shpere
	XMVECTOR sup_sphere = GetSupportCirecle(search_dir, sphere);
	//support of obb
	XMVECTOR sup_obb = GetSupportPoints(-search_dir, corners,8);

	c = sup_sphere - sup_obb;
	search_dir = -c;

	sup_sphere = GetSupportCirecle(search_dir, sphere);
	sup_obb = GetSupportPoints(-search_dir, corners,8);
	b = sup_sphere - sup_obb;

	if (XMVector3Greater(XMVectorZero(), XMVector3Dot(b, search_dir)))
	{
		return false;
	}

	search_dir = XMVector3Cross(XMVector3Cross(c - b, -b), c - b);
	if (XMVector3Equal(search_dir, XMVectorZero())) { //origin is on this line segment
		//Apparently any normal search vector will do?
		search_dir = XMVector3Cross(c - b, XMVectorSet(1, 0, 0, 0)); //normal with x-axis
		if (XMVector3Equal(search_dir, XMVectorZero()))
		{
			search_dir = XMVector3Cross(c - b, XMVectorSet(0, 0, -1, 0));
		}; //normal with z-axis
	}

	int simp_dim = 2; //simplex dimension
	int GJK_MAX_NUM_ITERATIONS = 64;

	for (int iterations = 0; iterations < GJK_MAX_NUM_ITERATIONS; iterations++)
	{
		sup_sphere = GetSupportCirecle(search_dir, sphere);
		sup_obb = GetSupportPoints(-search_dir, corners,8);
		a = sup_sphere - sup_obb;

		if (XMVector3Greater(XMVectorZero(),XMVector3Dot(a, search_dir)))
		{
			return false;//we didn't reach the origin, won't enclose it
		}

		simp_dim++;
		if (simp_dim == 3)
		{
			update_simplex3(a, b, c, d, simp_dim, search_dir);
		}
		else if (update_simplex4(a, b, c, d, simp_dim, search_dir))
		{
			mtv = EPA(a, b, c, d, obb, sphere);
			return true;
		}
	}//endfor
	return false;

	delete corners;
	return true;
}

XMVECTOR GJKCollision::GetSupportCirecle(const XMVECTOR& dir, const BoundingSphere& sphere) {
	XMVECTOR sup_sphere = DirectX::XMVector3Normalize(dir) * sphere.Radius + XMLoadFloat3(&sphere.Center);
	return sup_sphere;
}

XMVECTOR GJKCollision::GetSupportOBB(const XMVECTOR& dir, DirectX::XMFLOAT3* corners) {
	//auto m = DirectX::XMMatrixTranspose(matobb);
	//XMVECTOR transdir = XMVector3Transform(dir, m);
	XMFLOAT3 transdirfloat3; XMStoreFloat3(&transdirfloat3, dir);//transdir
	XMFLOAT3 sup_obbfloat3;
	transdirfloat3.x > 0 ? sup_obbfloat3.x = GetMaxX(corners) : sup_obbfloat3.x = GetMinX(corners);
	transdirfloat3.y > 0 ? sup_obbfloat3.y = GetMaxY(corners) : sup_obbfloat3.y = GetMinY(corners);
	transdirfloat3.z > 0 ? sup_obbfloat3.z = GetMaxZ(corners) : sup_obbfloat3.z = GetMinZ(corners);
	XMVECTOR sup_obb = XMLoadFloat3(&sup_obbfloat3);
	return sup_obb;
}

XMVECTOR GJKCollision::GetSupportPoints(const XMVECTOR& dir, DirectX::XMFLOAT3* corners ,int number)
{
	XMVECTOR furthest_point = XMLoadFloat3(&corners[0]);
	float max_dot = XMVectorGetX(XMVector3Dot(furthest_point, dir));

	for (size_t i = 1; i < number; i++)
	{
		XMVECTOR v = XMLoadFloat3(&corners[i]);
		float d = XMVectorGetX(XMVector3Dot(v, dir));
		if (d > max_dot) {
			max_dot = d;
			furthest_point = v;
		}
	}
	return furthest_point;
}

float GJKCollision::GetMaxX(DirectX::XMFLOAT3* corners)
{
	float rtn = -FLT_MAX;
	for (size_t i = 0; i < 8u; i++)
	{
		corners[i].x > rtn ? rtn = corners[i].x : rtn = rtn;
	}
	return rtn;
}

float GJKCollision::GetMinX(DirectX::XMFLOAT3* corners)
{
	float rtn = FLT_MAX;
	for (size_t i = 0; i < 8u; i++)
	{
		corners[i].x < rtn ? rtn = corners[i].x : rtn = rtn;
	}
	return rtn;
}

float GJKCollision::GetMaxY(DirectX::XMFLOAT3* corners)
{
	float rtn = -FLT_MAX;
	for (size_t i = 0; i < 8u; i++)
	{
		corners[i].y > rtn ? rtn = corners[i].y : rtn = rtn;
	}
	return rtn;
}

float GJKCollision::GetMinY(DirectX::XMFLOAT3* corners)
{
	float rtn = FLT_MAX;
	for (size_t i = 0; i < 8u; i++)
	{
		corners[i].y < rtn ? rtn = corners[i].y : rtn = rtn;
	}
	return rtn;
}

float GJKCollision::GetMaxZ(DirectX::XMFLOAT3* corners)
{
	float rtn = -FLT_MAX;
	for (size_t i = 0; i < 8u; i++)
	{
		corners[i].z > rtn ? rtn = corners[i].z : rtn = rtn;
	}
	return rtn;
}

float GJKCollision::GetMinZ(DirectX::XMFLOAT3* corners)
{
	float rtn = FLT_MAX;
	for (size_t i = 0; i < 8u; i++)
	{
		corners[i].z < rtn ? rtn = corners[i].z : rtn = rtn;
	}
	return rtn;
}

void GJKCollision::update_simplex3(XMVECTOR& a, XMVECTOR& b, XMVECTOR& c, XMVECTOR& d, int& simp_dim, XMVECTOR& search_dir)
{
	/* Required winding order:
	//  b
	//  | \
	//  |   \
	//  |    a
	//  |   /
	//  | /
	//  c
	*/
	XMVECTOR n = XMVector3Cross(b - a, c - a); //triangle's normal
	XMVECTOR AO = -a; //direction to origin

	//Determine which feature is closest to origin, make that the new simplex

	simp_dim = 2;
	if (XMVector3Greater(XMVector3Dot(XMVector3Cross(b - a, n), AO), XMVectorZero())) { //Closest to edge AB
		c = a;
		//simp_dim = 2;
		search_dir = XMVector3Cross(XMVector3Cross(b - a, AO), b - a);
		return;
	}
	if (XMVector3Greater(XMVector3Dot(XMVector3Cross(n, c - a), AO), XMVectorZero())) { //Closest to edge AC
		b = a;
		//simp_dim = 2;
		search_dir = XMVector3Cross(XMVector3Cross(c - a, AO), c - a);
		return;
	}

	simp_dim = 3;
	if (XMVector3Greater(XMVector3Dot(n, AO), XMVectorZero())) { //Above triangle
		d = c;
		c = b;
		b = a;
		//simp_dim = 3;
		search_dir = n;
		return;
	}
	//else //Below triangle
	d = b;
	b = a;
	//simp_dim = 3;
	search_dir = -n;
	return;
}

bool GJKCollision::update_simplex4(XMVECTOR& a, XMVECTOR& b, XMVECTOR& c, XMVECTOR& d, int& simp_dim, XMVECTOR& search_dir)
{
	// a is peak/tip of pyramid, BCD is the base (counterclockwise winding order)
   //We know a priori that origin is above BCD and below a

   //Get normals of three new faces
	XMVECTOR ABC = XMVector3Cross(b - a, c - a);
	XMVECTOR ACD = XMVector3Cross(c - a, d - a);
	XMVECTOR ADB = XMVector3Cross(d - a, b - a);

	XMVECTOR AO = -a; //dir to origin
	simp_dim = 3; //hoisting this just cause

	//Plane-test origin with 3 faces
	/*
	// Note: Kind of primitive approach used here; If origin is in front of a face, just use it as the new simplex.
	// We just go through the faces sequentially and exit at the first one which satisfies dot product. Not sure this
	// is optimal or if edges should be considered as possible simplices? Thinking this through in my head I feel like
	// this method is good enough. Makes no difference for AABBS, should test with more complex colliders.
	*/
	if (XMVector3Greater(XMVector3Dot(ABC, AO), XMVectorZero())) { //In front of ABC
		d = c;
		c = b;
		b = a;
		search_dir = ABC;
		return false;
	}
	if (XMVector3Greater(XMVector3Dot(ACD, AO), XMVectorZero())) { //In front of ACD
		b = a;
		search_dir = ACD;
		return false;
	}
	if (XMVector3Greater(XMVector3Dot(ADB, AO), XMVectorZero())) { //In front of ADB
		c = d;
		d = b;
		b = a;
		search_dir = ADB;
		return false;
	}

	//else inside tetrahedron; enclosed!
	return true;

	//Note: in the case where two of the faces have similar normals,
	//The origin could conceivably be closest to an edge on the tetrahedron
	//Right now I don't think it'll make a difference to limit our new simplices
	//to just one of the faces, maybe test it later.
}



//Expanding Polytope Algorithm
//Find minimum translation vector to resolve collision
XMVECTOR GJKCollision::EPA(XMVECTOR& a, XMVECTOR& b, XMVECTOR& c, XMVECTOR& d, const BoundingOrientedBox& obb, const BoundingSphere& sphere)
{
	XMVECTOR faces[EPA_MAX_NUM_FACES][4]; //Array of faces, each with 3 verts and a normal

	//Init with final simplex from GJK
	faces[0][0] = a;
	faces[0][1] = b;
	faces[0][2] = c;
	faces[0][3] = XMVector3Normalize(XMVector3Cross(b - a, c - a)); //ABC
	faces[1][0] = a;
	faces[1][1] = c;
	faces[1][2] = d;
	faces[1][3] = XMVector3Normalize(XMVector3Cross(c - a, d - a)); //ACD
	faces[2][0] = a;
	faces[2][1] = d;
	faces[2][2] = b;
	faces[2][3] = XMVector3Normalize(XMVector3Cross(d - a, b - a)); //ADB
	faces[3][0] = b;
	faces[3][1] = d;
	faces[3][2] = c;
	faces[3][3] = XMVector3Normalize(XMVector3Cross(d - b, c - b)); //BDC

	int num_faces = 4;
	int closest_face;

	for (int iterations = 0; iterations < EPA_MAX_NUM_ITERATIONS; iterations++)
	{
		//Find face that's closest to origin
		float min_dist = XMVectorGetX(XMVector3Dot(faces[0][0], faces[0][3]));
		closest_face = 0;
		for (int i = 1; i < num_faces; i++)
		{
			float dist = XMVectorGetX(XMVector3Dot(faces[i][0], faces[i][3]));
			if (dist < min_dist) {
				min_dist = dist;
				closest_face = i;
			}
		}

		//search normal to face that's closest to origin
		XMVECTOR search_dir = faces[closest_face][3];

		//support of shpere
		XMVECTOR sup_sphere = GetSupportCirecle(search_dir, sphere);
		//support of obb
		DirectX::XMFLOAT3* corners = (DirectX::XMFLOAT3*)malloc(sizeof(DirectX::XMFLOAT3) * 8u);
		obb.GetCorners((DirectX::XMFLOAT3*)corners);
		XMVECTOR sup_obb = GetSupportOBB(-search_dir, corners);
		delete corners;

		XMVECTOR p = sup_sphere - sup_obb;

		if (XMVectorGetX(XMVector3Dot(p, search_dir)) - min_dist < EPA_TOLERANCE)
		{
			//Convergence (new point is not significantly further from origin)
			return faces[closest_face][3] * XMVector3Dot(p, search_dir); //dot vertex with normal to resolve collision along normal!
		}

		XMVECTOR loose_edges[EPA_MAX_NUM_LOOSE_EDGES][2]; //keep track of edges we need to fix after removing faces
		int num_loose_edges = 0;

		//Find all triangles that are facing p
		for (int i = 0; i < num_faces; i++)
		{
			if (XMVector3Greater(XMVector3Dot(faces[i][3], p - faces[i][0]), XMVectorZero())) //triangle i faces p, remove it
			{
				//Add removed triangle's edges to loose edge list.
				//If it's already there, remove it (both triangles it belonged to are gone)
				for (int j = 0; j < 3; j++) //Three edges per face
				{
					XMVECTOR current_edge[2] = { faces[i][j], faces[i][(j + 1) % 3] };
					bool found_edge = false;
					for (int k = 0; k < num_loose_edges; k++) //Check if current edge is already in list
					{
						if (XMVector3Equal(loose_edges[k][1], current_edge[0]) && XMVector3Equal(loose_edges[k][0], current_edge[1])) {
							//Edge is already in the list, remove it
							//THIS ASSUMES EDGE CAN ONLY BE SHARED BY 2 TRIANGLES (which should be true)
							//THIS ALSO ASSUMES SHARED EDGE WILL BE REVERSED IN THE TRIANGLES (which 
							//should be true provided every triangle is wound CCW)
							loose_edges[k][0] = loose_edges[num_loose_edges - 1][0]; //Overwrite current edge
							loose_edges[k][1] = loose_edges[num_loose_edges - 1][1]; //with last edge in list
							num_loose_edges--;
							found_edge = true;
							k = num_loose_edges; //exit loop because edge can only be shared once
						}
					}//endfor loose_edges

					if (!found_edge) { //add current edge to list
						// assert(num_loose_edges<EPA_MAX_NUM_LOOSE_EDGES);
						if (num_loose_edges >= EPA_MAX_NUM_LOOSE_EDGES) break;
						loose_edges[num_loose_edges][0] = current_edge[0];
						loose_edges[num_loose_edges][1] = current_edge[1];
						num_loose_edges++;
					}
				}

				//Remove triangle i from list
				faces[i][0] = faces[num_faces - 1][0];
				faces[i][1] = faces[num_faces - 1][1];
				faces[i][2] = faces[num_faces - 1][2];
				faces[i][3] = faces[num_faces - 1][3];
				num_faces--;
				i--;
			}//endif p can see triangle i
		}//endfor num_faces

		//Reconstruct polytope with p added
		for (int i = 0; i < num_loose_edges; i++)
		{
			// assert(num_faces<EPA_MAX_NUM_FACES);
			if (num_faces >= EPA_MAX_NUM_FACES) break;
			faces[num_faces][0] = loose_edges[i][0];
			faces[num_faces][1] = loose_edges[i][1];
			faces[num_faces][2] = p;
			faces[num_faces][3] = XMVector3Normalize((XMVector3Cross(loose_edges[i][0] - loose_edges[i][1], loose_edges[i][0] - p)));

			//Check for wrong normal to maintain CCW winding
			float bias = 0.000001; //in case dot result is only slightly < 0 (because origin is on face)
			if (XMVectorGetX(XMVector3Dot(faces[num_faces][0], faces[num_faces][3])) + bias < 0) {
				XMVECTOR temp = faces[num_faces][0];
				faces[num_faces][0] = faces[num_faces][1];
				faces[num_faces][1] = temp;
				faces[num_faces][3] = -faces[num_faces][3];
			}
			num_faces++;
		}
	} //End for iterations
	//printf("EPA did not converge\n");
	//Return most recent closest point

	return faces[closest_face][3] * XMVector3Dot(faces[closest_face][0], faces[closest_face][3]);
}
