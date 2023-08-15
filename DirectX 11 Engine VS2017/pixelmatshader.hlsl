static const float PI = 3.14159265359;

cbuffer alphaBuffer : register(b0)
{
	float3 cameraPosition;
    float alpha;

	float3 albedo;
	float cbmetallic;
	float cbroughness;
	float ao;
	float normaltype;
}

cbuffer lightDataBuffer : register(b1)
{
	float3 lightPos1;
	float padding1;
	float3 lightPos2;
	float padding2;
	float3 lightPos3;
	float padding3;
	float3 lightPos4;
	float padding4;
	float3 lightCol;
	float padding5;
}

cbuffer BSDFConstantBuffer : register(b2)
{
	float3 baseColor;
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
}

struct cb_ps_HeightMapStatus
{
	float HeightScale;
	float MaxTessDistance;
	float MinTessDistance;
	float MinTessFactor;
	float MaxTessFactor;
};

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;

	float3 inNormal : NORMAL;
	float3 inTangent: TANGENT;
	float3 inWorldPos : POSITION;
};

SamplerState basicSampler : SAMPLER : register(s0);

TextureCube skyIR : register(t0);
TextureCube skyPrefilter: register(t1);
Texture2D brdfLUT : TEXTURE: register(t2);

Texture2D objTexture : TEXTURE: register(t3);
Texture2D normalSRV : TEXTURE: register(t4);
Texture2D metallicSRV : TEXTURE: register(t5);
Texture2D roughSRV : TEXTURE: register(t6);
Texture2D heightSRV : TEXTURE: register(t7);

Texture2D dissolveNoiseTexture : TEXTURE: register(t8);

Texture2D emissionSRV : TEXTURE: register(t9);
Texture2D aoSRV : TEXTURE: register(t10);


//======================================================
float sqr(float x) { return x * x; }

float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay)
{
	return 1 / (NdotV + sqrt(sqr(VdotX*ax) + sqr(VdotY*ay) + sqr(NdotV)));
}

float SchlickFresnel(float u)
{
	float m = clamp(1 - u, 0, 1);
	float m2 = m * m;
	return m2 * m2*m; // pow(m,5)
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
{
	return 1 / (PI * ax*ay * sqr(sqr(HdotX / ax) + sqr(HdotY / ay) + NdotH * NdotH));
}

float GTR1(float NdotH, float a)
{
	if (a >= 1) return 1 / PI;
	float a2 = a * a;
	float t = 1 + (a2 - 1)*NdotH*NdotH;
	return (a2 - 1) / (PI*log(a2)*t);
}

float smithG_GGX(float NdotV, float alphaG)
{
	float a = alphaG * alphaG;
	float b = NdotV * NdotV;
	return 1 / (NdotV + sqrt(a + b - a * b));
}
//======================================================

float NormalDistributionGGXTR(float3 normalVec, float3 halfwayVec, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;   // a2 = a^2
	float NdotH = max(dot(normalVec, halfwayVec), 0.0);     // NdotH = normalVec.halfwayVec
	float NdotH2 = NdotH * NdotH;   // NdotH2 = NdotH^2
	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)  // k is a remapping of roughness based on direct lighting or IBL lighting
{
	float r = roughness + 1.0f;
	float k = (r * r) / 8.0f;

	float nom = NdotV;
	float denom = NdotV * (1.0f - k) + k;

	return nom / denom;
}

float GeometrySmith(float3 normalVec, float3 viewDir, float3 lightDir, float k)
{
	float NdotV = max(dot(normalVec, viewDir), 0.0f);
	float NdotL = max(dot(normalVec, lightDir), 0.0f);
	float ggx1 = GeometrySchlickGGX(NdotV, k);
	float ggx2 = GeometrySchlickGGX(NdotL, k);

	return ggx1 * ggx2;
}

float3 FresnelSchlick(float cosTheta, float3 F0)   // cosTheta is n.v and F0 is the base reflectivity
{
	return (F0 + (1.0f - F0) * pow(1.0 - cosTheta, 5.0f));
}

float3 FresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)   // cosTheta is n.v and F0 is the base reflectivity
{
	return F0 + (max(float3(1.0f - roughness, 1.0f - roughness, 1.0f - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0f);
}

void CalcRadiance(PS_INPUT input, float3 viewDir, float3 normalVec, float3 albedo, float roughness, float metallic, float3 lightPos, float3 lightCol, float3 F0, out float3 rad)
{
	//static const float PI = 3.14159265359;

	//calculate light radiance
	float3 lightDir = normalize(lightPos - input.inWorldPos);
	float3 halfwayVec = normalize(viewDir + lightDir);
	float distance = length(lightPos - input.inWorldPos);
	float attenuation = 1.0f / (distance * distance);// 
	float3 radiance = lightCol * attenuation;

	//Cook-Torrance BRDF
	float D = NormalDistributionGGXTR(normalVec, halfwayVec, roughness);
	float G = GeometrySmith(normalVec, viewDir, lightDir, roughness);
	float3 F = FresnelSchlick(max(dot(halfwayVec, viewDir), 0.0f), F0);

	float3 kS = F;
	float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
	kD *= 1.0 - metallic;

	float3 nom = D * G * F;
	float denom = 4 * max(dot(normalVec, viewDir), 0.0f) * max(dot(normalVec, lightDir), 0.0) + 0.001f; // 0.001f just in case product is 0
	float3 specular = nom / denom;

	//Add to outgoing radiance Lo
	float NdotL = max(dot(normalVec, lightDir), 0.0f);
	rad = (((kD * albedo / PI) + specular) * radiance * NdotL);
}

float3 aces_tonemap(float3 color) {
	float3x3 m1 = float3x3(
		0.59719, 0.07600, 0.02840,
		0.35458, 0.90834, 0.13383,
		0.04823, 0.01566, 0.83777
	);
	float3x3 m2 = float3x3(
		1.60475, -0.10208, -0.00327,
		-0.53108, 1.10813, -0.07276,
		-0.07367, -0.00605, 1.07602
	);
	float3 v = mul(color, m1);
	float3 a = v * (v + 0.0245786) - 0.000090537;
	float3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
	float3 r = mul((a / b), m2);
	r = clamp(r, 0.0, 1.0);
	return pow(r, 1.0 / 2.2);
}



float4 main(PS_INPUT input) : SV_TARGET
{
	if(usepurecolor == 0){
		
		//float3 pixelColor = objTexture.Sample(objSamplerState, input.inTexCoord);
		float3 pixelColor = objTexture.Sample(basicSampler, input.inTexCoord).rgb;//pow(, 2.2f)

		//Normal
		float3 N, T, B;
		float3x3 TBN;

		float3 normalFromMap;
		if (normaltype == 0)
		{
			normalFromMap = normalSRV.Sample(basicSampler, input.inTexCoord).xyz * 2 - 1;
			
			N = input.inNormal;
			T = normalize(input.inTangent - N * dot(input.inTangent, N));
			B = cross(T, N);
			TBN = float3x3(T, B, N);
			input.inNormal = normalize(mul(normalFromMap, TBN));
		}
		else if (normaltype == 1)
		{
			normalFromMap = normalSRV.Sample(basicSampler, input.inTexCoord).xyz;

			float3 Q1 = ddx(input.inWorldPos);
			float3 Q2 = ddy(input.inWorldPos);
			float2 st1 = ddx(input.inTexCoord);
			float2 st2 = ddy(input.inTexCoord);

			N = normalize(input.inNormal);
			T = normalize(Q1*st2.x - Q2 * st1.x);
			B = -normalize(cross(N, T));
			TBN = float3x3(T, B, N);
			input.inNormal = normalize(mul(normalFromMap, TBN));

		}

		float3 normalVec = input.inNormal;

		//float3 tangentNormal = normalSRV.Sample(basicSampler, input.inTexCoord).xyz * 2 - 1;

		//float3 Q1 = ddx(input.inWorldPos);
		//float3 Q2 = ddy(input.inWorldPos);
		//float2 st1 = ddx(input.inTexCoord);
		//float2 st2 = ddy(input.inTexCoord);

		////float3 N = normalize(input.inNormal);
		//float3 T = normalize(Q1*st2.x - Q2 * st1.x);
		//float3 B = -normalize(cross(N, T));
		//float3x3 TBN = float3x3(T, B, N);

		//float3 normalVec = normalize(mul(TBN , tangentNormal));

		//Metallic
		float metallic = cbmetallic * metallicSRV.Sample(basicSampler, input.inTexCoord).r;

		//Rough
		float rough = cbroughness * roughSRV.Sample(basicSampler, input.inTexCoord).r;

		//Emission
		float3 emission = emissionSRV.Sample(basicSampler, input.inTexCoord).rgb;

		float3 viewDir = normalize(cameraPosition - input.inWorldPos);

		float3 R = reflect(-viewDir, normalVec);

		float3 F0 = float3(0.04f, 0.04f, 0.04f);
		F0 = lerp(F0, pixelColor * baseColor, metallic);

		float3 rad = float3(0.0f, 0.0f, 0.0f);
		//reflectance equation
		float3 Lo = float3(0.0f, 0.0f, 0.0f);

		CalcRadiance(input, viewDir, normalVec, pixelColor, rough, metallic, lightPos1, lightCol, F0, rad);
		Lo += rad;

		CalcRadiance(input, viewDir, normalVec, pixelColor, rough, metallic, lightPos2, lightCol, F0, rad);
		Lo += rad;

		CalcRadiance(input, viewDir, normalVec, pixelColor, rough, metallic, lightPos3, lightCol, F0, rad);
		Lo += rad;

		CalcRadiance(input, viewDir, normalVec, pixelColor, rough, metallic, lightPos4, lightCol, F0, rad);
		Lo += rad;

		float3 kS = FresnelSchlickRoughness(max(dot(normalVec, viewDir), 0.0f), F0, rough);
		float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
		kD *= 1.0 - metallic;

		float3 irradiance = skyIR.Sample(basicSampler, normalVec).rgb;
		float3 diffuse = pixelColor * baseColor * irradiance;//

		const float MAX_REF_LOD = 4.0f;
		float3 prefilteredColor = skyPrefilter.SampleLevel(basicSampler, R, rough * MAX_REF_LOD).rgb;
		float2 brdf = brdfLUT.Sample(basicSampler, float2(max(dot(normalVec, viewDir), 0.0f), rough)).rg;
		float3 specular = prefilteredColor * (kS * brdf.x + brdf.y);
		
		float aomap = aoSRV.Sample(basicSampler, input.inTexCoord).g;
		float3 ambient;
		if (aomap == 0)
		{
			ambient = (kD * diffuse + specular) * ao;
		}
		else 
		{
			ambient = (kD * diffuse + specular) * aomap;
		}
		
	
		//=================================================================
		float3 L = normalize(lightPos1 - input.inWorldPos);
		float3 V = normalize(cameraPosition - input.inWorldPos);

		float NdotL = dot(N, L);
		float NdotV = dot(N, V);

		float3 H = normalize(L + V);
		float NdotH = dot(N, H);
		float LdotH = dot(L, H);

		float aspect = sqrt(1 - anisotropic * .9);
		float ax = max(.001, sqr(rough) / aspect);
		float ay = max(.001, sqr(rough) * aspect);

		float Gs;
		Gs = smithG_GGX_aniso(NdotL, dot(L, T), dot(L, B), ax, ay);
		Gs *= smithG_GGX_aniso(NdotV, dot(V, T), dot(V, B), ax, ay);

		float3 Cdlin = pixelColor;
		float Cdlum = .3 * Cdlin[0] + .6 * Cdlin[1] + .1 * Cdlin[2];

		float3 Ctint = Cdlum > 0 ? Cdlin / Cdlum : float3(1, 1, 1);
		float3 Cspec0 = lerp(specular *.08 * lerp(float3(1, 1, 1), Ctint, specularTint), Cdlin, metallic);
		float3 Csheen = lerp(float3(1, 1, 1), Ctint, sheenTint);
		float FH = SchlickFresnel(LdotH);
		float3 Fs = lerp(Cspec0, float3(1, 1, 1), FH);
		float Ds = GTR2_aniso(NdotH, dot(H, T), dot(H, B), ax, ay);

		float Dr = GTR1(NdotH, lerp(.1, .001, clearcoatGloss));
		float Fr = lerp(.04, 1.0, FH);
		float Gr = smithG_GGX(NdotL, .25) * smithG_GGX(NdotV, .25);

		//=================================================================

		float3 color = ambient + Lo + Gs * Fs * Ds + .25 * clearcoat * Gr * Fr * Dr;//emission

		//color = aces_tonemap(color);
		color = color / (color + float3(1.0f, 1.0f, 1.0f));
		color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f)) + emission;

		return float4(color, alpha); 
	}
	else if (usepurecolor == 1)
	{
		return float4(baseColor, alpha);
	}


	return float4(baseColor, alpha);
}