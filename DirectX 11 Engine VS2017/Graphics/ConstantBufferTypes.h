#pragma once
#include <DirectXMath.h>

struct CB_VS_vertexshader
{
	DirectX::XMMATRIX mat;
	DirectX::XMMATRIX mat_wvp;
};

struct CB_PS_pixelshader
{
	DirectX::XMFLOAT3 cameraPosition;
	float alpha = 1.0f;

	DirectX::XMFLOAT3 albedo;
	float metallic;
	float roughness;
	float ao;
	float normaltype;
	float padding;
};

struct CB_PS_lightData 
{
	DirectX::XMFLOAT3 lightPos1;
	float padding1;
	DirectX::XMFLOAT3 lightPos2;
	float padding2;
	DirectX::XMFLOAT3 lightPos3;
	float padding3;
	DirectX::XMFLOAT3 lightPos4;
	float padding4;
	DirectX::XMFLOAT3 lightCol;
	float padding5;
};

struct CB_PS_IBLSTATUS
{
	float roughness;
	float metallic;
};

struct cb_ps_BSDFData
{
	DirectX::XMFLOAT3 baseColor;
	float metallic;
	float subsurface;
	float specular;
	float roughness;
	float specularTint;
	float anisotropic;
	float sheen;
	float sheenTint;
	float clearcoat;
	float clearcoatGloss;
	float usepurecolor;
}; 

struct cb_ps_HeightMapStatus
{
	float HeightScale;
	float MaxTessDistance;
	float MinTessDistance;
	float MinTessFactor;
	float MaxTessFactor;
};
