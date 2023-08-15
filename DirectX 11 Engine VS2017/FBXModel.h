#pragma once
#include <fbxsdk.h>
#include "BitFlag.h"
#include "FBXDefine.h"
#include "ErrorLogger.h"
#include <vector>

class FBXModel
{
public:
	FBXModel();
	~FBXModel();

	const FBXMeshData* GetMeshData() const;
	const FBXControlPointGroup* GetControlPointGroup() const;
	const FBXBoneGroup* GetBoneGroup() const;
	const FBXModelAnimation* GetAnimation() const;
	unsigned int GetAnimationSize();

	unsigned int m_kcurrentAnimaionIndex = 0;
	unsigned int m_kpreAnimaionIndex = -1;
	float preanimtime = 0;
	unsigned int prekeyframeindex = 0;
	//
	const FBXMeshData* GetAnimationMeshData(float fTime);
	const FBXMeshData* GetAnimationMeshDataNew(float fTime);
	float GetCurrentAnimationTime(float fTime);
	//Ŀǰģ����ֻ��һ����������ȡ�������ж��ٸ��ؼ�֡��
	int GetKeyFrameCount() const;
	float GetAnimTimeLength();
	//����Mesh�İ�Χ�С�
	void CalculateMeshBoundingBox(XMFLOAT3* pMinPos, XMFLOAT3* pMaxPos);

	void CreateFBXStatus(const char* szFileName, FBXModel* pFBXModel);
	void CreateFBXAnimationOnly(const char* szFileName, FBXModel* pFBXModel);
	void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
	bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);
	void DestroySdkObjects(FbxManager* pManager, bool pExitStatus);
	void ParseMeshVertexTotalCountAndVertexType(FbxNode* pNode, BitFlag* pVertexType, int* pTotalCount, bool* pAllMeshOK);
	void ParseSingleMeshVertexType(FbxMesh* pFbxMesh, BitFlag* pVertexType, bool* pAllMeshOK, bool bFirstMesh);
	void ParseMeshData(FbxNode* pNode, FBXMeshData* pMeshData, int* pProcessedVertexCount, int* pProcessedControlPointCount);
	void LoadSingleMesh(FbxMesh* pFbxMesh, FBXMeshData* pMeshData, int* pProcessedVertexCount, int* pProcessedControlPointCount);
	void LoadSingleMesh_Normal(FbxMesh* pFbxMesh, int nVertexIndex, FbxVector4* pNormal);
	void LoadSingleMesh_Tangent(FbxMesh* pFbxMesh, int nVertexIndex, FbxVector4* pTangent);
	void LoadSingleMesh_Color(FbxMesh* pFbxMesh, int nVertexIndex, FbxColor* pColor);
	void LoadSingleMesh_UV(FbxMesh* pFbxMesh, int nUVIndex, int nVertexIndex, FbxVector2* pUV);
	void ParseControlPointTotalCount(FbxNode* pNode, int* pTotalCount);
	void ParseAllControlPoint(FbxNode* pNode, FBXControlPointGroup* pControlPointGroup);
	void LoadControlPointFromMesh(FbxMesh* pMesh, FBXControlPointGroup* pControlPointGroup);
	void ParseAllControlPointSkinWeight(FbxNode* pNode, FBXControlPointGroup* pControlPointGroup, int* pProcessedControlPointCount);
	void LoadSingleControlPointSkinWeight(FbxMesh* pMesh, FBXControlPointGroup* pControlPointGroup, int* pProcessedControlPointCount);
	void ParseBoneTotalCount(FbxNode* pNode, int* pTotalCount);
	void ParseBoneHierarchy(FbxNode* pNode, FBXBoneGroup* pGroup, int nParentIndex);
	void ParseAllBoneMatrixFromBoneSpaceToWorldSpace(FbxNode* pNode, FbxNode* pRootNode, FBXBoneGroup* pBoneGroup);
	void GetGeometryTransformation(FbxNode* pNode, FbxAMatrix* pMat);
	void LoadSingleBoneMatrixFromBoneSpaceToWorldSpace(FbxNode* pNode, FbxMesh* pMesh, FbxNode* pRootNode, FBXBoneGroup* pBoneGroup);
	void FindBoneByIndexAndName(FbxNode* pNode, int nBoneIndex, const char* szBoneName, FbxNode** ppResultNode, int* pAccBoneCount);
	void ConvertFbxAMatrixToDirectXMatrix(const FbxAMatrix* pFbxAMatrix, XMMATRIX* pSoMatrix);
	void ParseAllBoneAnimationData(FbxScene* pSDKScene, FBXBoneGroup* pBoneGroup);//, FBXModelAnimation* pModelAnim
	void ParseSingleBoneAnimationData(FbxScene* pSDKScene, const FBXBoneGroup* pBoneGroup);

protected:
	FBXMeshData* GetMeshData_Modify();
	FBXControlPointGroup* GetControlPointGroup_Modify();
	FBXBoneGroup* GetBoneGroup_Modify();
	FBXModelAnimation* GetAnimation_Modify();
	void CalculateMeshDataByKeyFrame(const int nKeyFrameIndex);
	void CalculateMeshDataByKeyFrameNew(const int nKeyFrameIndex, float curTime, const int nPreKeyFrameIndex, float preTime);

private:
	//�������ݺ������Mesh������Ⱦ��������Ĭ�ϵ�Pose��
	//��ִ��CalculateMeshDataByKeyFrame()�������Mesh���ݾͱ����ָ��֡��Pose��
	FBXMeshData m_kMeshData;
	FBXControlPointGroup m_kControlPointGroup;
	FBXBoneGroup m_kBoneGroup;
	//FBXModelAnimation m_kAnimation;
	std::vector<FBXModelAnimation*> m_kAnimations;


	
};