//----------------------------------------------------------------
#ifndef _FBXDefine_h_
#define _FBXDefine_h_
//----------------------------------------------------------------
#include <DirectXMath.h>
#include <string>
#include "BitFlag.h"

using namespace DirectX;
//----------------------------------------------------------------
//1������Լ����һ��fbx�ļ��У����е�Mesh�Ķ������ݸ�ʽ����ͬ��
//   Ҳ�����������а����ĳ�Ա�����ߣ�����ɫ��UV������ͬ��
//   ��ģ������е�Mesh�Ķ������ݴ洢��ͬһ��VertexBuff�У�ֻ��һ��
//   DrawCall�Ͱ�ģ����Ⱦ������
//2��fbx�ļ��п��԰���һ��Mesh����Ҳ���԰������Mesh���󣬱�ģ��
//   ���ܹ���ȷ������
//3�����������г�Ա�Ĵ洢˳������� ����-����-����-����ɫ-UV1-UV2 ��
//   Ŀǰ���ڵ���kfm�ļ�ʱҪ�������˳��
//----------------------------------------------------------------
//��������Ƭ�����������з�ʽ��Ҳ��������˳ʱ�뻹����ʱ�롣
//��fbx�ļ��в���ʶ��˳ʱ�����ʱ�룬�����á�123˳�򹹳������Ρ��͡�321˳�򹹳������Ρ���
//�����ֵΪ1����ʾʹ�á�123˳�򹹳������Ρ���ֵΪ0��ʾʹ�á�321˳�򹹳������Ρ���
#define FBX_TriangleIndex_123 0
//----------------------------------------------------------------
//UV�����е�yֵ�Ƿ�ת��Ϊȡ������ȡֵΪ(1-y)��
//ֵΪ1����ʾҪ��ԭ����yֵת��Ϊ(1-y)��
//������Ϊ�����ҵ���Ⱦ����У���ͼ����ϵ��(0,0)��λ�����Ͻǣ�������ͼ�������������С�
//��ĳЩ��Ⱦ����У����ܳ��֡���ͼ����ϵ��(0,0)��λ�����½ǣ�������ͼ�������������С���
//���ߡ���ͼ����ϵ��(0,0)��λ�����Ͻǣ�������ͼ�����ǵ��õģ����µߵ��ġ���
//����������£�����Ҫ�����������ֵ��
#define FBX_UVHeightInverse 1
//----------------------------------------------------------------
//�Ƿ���ȡfbx�ļ��еķ�������
#define FBX_ParseNormalData 1
//�Ƿ���ȡfbx�ļ��е���������
#define FBX_ParseTangentData 1
//�Ƿ���ȡfbx�ļ��еĶ���ɫ����
#define FBX_ParseColorData 0
//�Ƿ���ȡfbx�ļ��еĵ�һ��UV����
#define FBX_ParseUV1Data 1
//�Ƿ���ȡfbx�ļ��еĵڶ���UV����
#define FBX_ParseUV2Data 0
//----------------------------------------------------------------
//ÿ�����Ƶ�����ܼ�������Ӱ�졣
#define FBX_MaxCount_BoneIndexSkinWeightPerControlPoint 4
//һ����������ж��ٸ��ӹ�����
//Ŀǰ���ӹ����б�û�з������á�
#define FBX_MaxCount_ChildBone 10

//----------------------------------------------------------------
//Vector3��3��floatԪ��
#define FBX_Sizeof_Vector3 12
//Color��4��floatԪ��
#define FBX_Sizeof_Color 16
//UV��2��floatԪ��
#define FBX_Sizeof_UV 8
//����Pixel��Sizeof
#define FBX_Sizeof_Pixel 4
//������ŵ�sizeofֵ��32λ���͡�
#define FBX_Sizeof_VertexIndex 4
//----------------------------------------------------------------
enum FBXElementType
{
	FBXElement_Position = 0x00000001,
	FBXElement_Normal = 0x00000002,
	FBXElement_Tangent = 0x00000004,
	FBXElement_Color = 0x00000008,
	FBXElement_UV1 = 0x00000010,
	FBXElement_UV2 = 0x00000020,
};
//----------------------------------------------------------------
//�洢fbx�ļ������е�Mesh�Ķ������ݡ�
//�ڽ���fbx�ļ���Ϻ�������洢�Ķ������ݾ���ģ��Ĭ��Pose�Ķ������ݡ�
//����ṹ������붥��������������ͬ�ġ�
//�����ζ�ţ����һ�����㱻�������������ι��õĻ�������������ݾͱ������������ݶ���ṹ�塣
struct FBXMeshData
{
	char* pVertexBuff;
	int nVertexBuffSize;
	int nVertexCount;
	char* pIndexBuff;
	int nIndexBuffSize;
	int nIndexCount;
	//����λ����ŵ����Ƶ�λ����ŵ�ӳ�䡣
	//������ĳ��Ⱦ���nVertexCount��
	int* pVertexIndex2ControlPointIndex;
	//����ṹ���к�����Щ��Ա�����ߣ���ͼ����ȵȣ���
	BitFlag kVertexType;
	//����ṹ����ֽ�����
	//����Լ����һ��fbx�ļ��У����е�Mesh�Ķ������ݸ�ʽ����ͬ��
	//��ͬ��mesh�У�����ṹ��ĳ�Ա��ͬ������ṹ����ֽ���Ҳ��ͬ��
	int nSizeofVertexData;

	FBXMeshData();
	~FBXMeshData();
	void SetVertexType(const BitFlag& kType);
	void ReserveVertexCount(const int nVertexCount);
};
//----------------------------------------------------------------
//�洢������ź͸ù�����ĳ�����Ƶ��Ӱ��Ȩ�ء�
//BoneIndex����StFBXBoneGroup�е������±ꡣ
struct FBXBoneIndexSkinWeight
{
	std::string kBoneName;
	int nBoneIndex;
	float fSkinWeight;

	FBXBoneIndexSkinWeight();
};
//----------------------------------------------------------------
//�������Ƶ㡣
struct FBXControlPoint
{
	//�������ꡣ
	XMFLOAT3 kVertex;
	//�����Ƶ�����Щ������Ӱ�죬�Լ�Ӱ��Ȩ�ء�
	FBXBoneIndexSkinWeight kPairList[FBX_MaxCount_BoneIndexSkinWeightPerControlPoint];

	FBXControlPoint();
	void AddBoneNameSkinWeight(const char* szBoneName, float fSkinWeight);
};
//----------------------------------------------------------------
//�洢fbx�����е�Mesh�Ŀ��Ƶ㡣
struct FBXBoneGroup; //ǰ������
struct FBXControlPointGroup
{
private:
	FBXControlPoint* kControlPointArray;
	int nControlPointValidCount;
	int nControlPointMaxCount;

public:
	FBXControlPointGroup();
	~FBXControlPointGroup();
	void ReserveControlPointCount(int nCount);
	//����緵��һ��δʹ�õ� StFBXControlPoint �����������丳ֵ��
	FBXControlPoint* TakeNew();
	FBXControlPoint* GetAt(int nIndex) const;
	int GetSize() const;
	//�ʼ��StFBXBoneIndexSkinWeight�����д洢���ǹ������֡�
	//�����˹������ݺ󣬾ͽ���������ת������Ӧ�Ĺ�����š�
	void MakeBoneIndexByBoneName(const FBXBoneGroup* pBoneGroup);

	//���ÿ�����Ƶ���ܹ���Ӱ��Ȩ�أ���֤Ȩ�ص��ܺ���1��
	//��������Ϊ����Ҫ��֤Ȩ�ص��ܺ���1��
	//���磬��һ�����㣬��ֻ��������������΢��Ӱ�죬
	//Ȩ��ֵ�Ƚ�С�������֡���΢������ôȨ�ص��ܺͱض�����1��
	//void CheckBoneSkinWeight();
};
//----------------------------------------------------------------
//�����ڵ�
struct FBXBone
{
	//�����������֡�
	std::string kBoneName;
	//�������ڲ㼶�б��е����λ�á�
	int nParentIndex;
	//�ӹ����ڲ㼶�б��е����λ�á�ÿ��Ԫ�ض���һ��intֵ��
	int kChildIndexList[FBX_MaxCount_ChildBone];
	//�������Ź����У���������ļ��㹫ʽ��
	//VertexAtTimeT = VertexFromControlPoint * kMatFromBoneSpaceToWorldSpace * KeyFrameMatrixAtTimeT
	XMMATRIX kMatFromBoneSpaceToModelSpace;

	FBXBone();
	~FBXBone();
};
//----------------------------------------------------------------
//�洢fbx�ļ������е�Bone�������㼶�ṹ Hierarchy��
struct FBXBoneGroup
{
private:
	FBXBone* kBoneArray;
	int nBoneValidCount;
	int nBoneMaxCount;

public:
	FBXBoneGroup();
	~FBXBoneGroup();
	void ReserveBoneCount(int nCount);
	//���������б��ֱ�����ÿ���������ӹ����б�
	void GenerateChildren();
	int GetBoneIndexByBoneName(const char* szBoneName) const;
	//����緵��һ��δʹ�õ� StFBXBone �����������丳ֵ��
	FBXBone* TakeNew(int* pIndex);
	FBXBone* GetAt(int nIndex) const;
	int GetSize() const;
};
//----------------------------------------------------------------
//����֡
struct FBXKeyFrame
{
	float fKeyTime;
	XMMATRIX matKeyTransform;

	FBXKeyFrame();
};
//----------------------------------------------------------------
//��һ�������У�һ��Bone��Я�������еĶ���֡��
//BoneIndex����StFBXBoneGroup�е������±ꡣ
struct FBXBoneAnimation
{
	FBXKeyFrame* kKeyFrameArray;
	int nFrameValidCount;
	int nFrameMaxCount;
	int nBoneIndex;

	FBXBoneAnimation();
	~FBXBoneAnimation();
	void ReserveKeyFrameCount(int nCount);
	//����緵��һ��δʹ�õ� StFBXKeyFrame �����������丳ֵ��
	FBXKeyFrame* TakeNew();
	FBXKeyFrame* GetAt(int nIndex) const;
	int GetSize() const;
};
//----------------------------------------------------------------
//��һ�������У�һ��ģ����Я�������еĶ���֡��
struct FBXModelAnimation
{
	//���й����Ķ���֡�б�ÿ��Ԫ�ض���һ��StFBXBoneAnimation����
	//һ������£��������size���ǹ����ܸ�����
	//�п��ܳ�������������ڶ����У�ֻ��һ���ֹ������������ã�
	//��ô�������size�ͻ�С�ڹ����ܸ�����
	FBXBoneAnimation* kBoneAnimationArray;
	int nAnimValidCount;
	int nAnimMaxCount;
	//�����ж���֡��
	int nFrameCount;
	//�����ĳ���ʱ������λ�롣
	float fAnimLength;

	FBXModelAnimation();
	~FBXModelAnimation();
	void ReserveBoneCount(int nBoneCount);
	int GetSize() const;
	int GetFrameCount() const;
	//����緵��һ��δʹ�õ� StFBXBoneAnimation �����������丳ֵ��
	FBXBoneAnimation* TakeNew();
	FBXBoneAnimation* GetAt(int nIndex) const;
	FBXBoneAnimation* GetBoneAnimation(int nBoneIndex) const;
	//����ָ����ʱ�䣬��ȡ��ʱ��Ҫ���Ŷ�������һ֡��
	int GetKeyFrameIndexByTime(float fTime);
};
//----------------------------------------------------------------
#endif //_FBXDefine_h_
//----------------------------------------------------------------
