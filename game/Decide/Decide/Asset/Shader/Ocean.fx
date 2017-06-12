/**
* 海シェーダ.
*/
#include"LightingFunction.h"

//法線マップ.
texture g_Normal_1;
sampler g_NormalSampler_1 =
sampler_state
{
	Texture = <g_Normal_1>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
//法線マップ.
texture g_Normal_2;
sampler g_NormalSampler_2 =
sampler_state
{
	Texture = <g_Normal_2>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

/** ワールド行列. */
float4x4 g_WorldMatrix;
/** ビュー行列. */
float4x4 g_ViewMatrix;
/** プロジェクション行列. */
float4x4 g_ProjMatrix;

/** 波. */
float g_Wave;

/**
* 入力頂点.
*/
struct VS_INPUT
{
	float4 Pos		: POSITION;		//!< 頂点座標.
	float2 Tex		: TEXCOORD0;	//!< UV座標.
	float4 Normal	: NORMAL;		//!< 法線ベクトル.
	float4 Tangent	: TANGENT;		//!< 接ベクトル.
};

struct VS_OUTPUT
{
	float4 Pos		: POSITION;		//!< 頂点座標.
	float2 Tex		: TEXCOORD0;	//!< UV座標.
	float4 Normal	: TEXCOORD1;	//!< 法線ベクトル.
	float4 Tangent	: TEXCOORD2;	//!< 接ベクトル.
	float4 WorldPos	: TEXCOORD3;	//!< ワールド座標.
};

/**
* 頂点シェーダ.
*/
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float4 pos = In.Pos;

	pos = mul(pos, g_WorldMatrix);

	Out.WorldPos = pos;

	pos = mul(pos, g_ViewMatrix);
	pos = mul(pos, g_ProjMatrix);

	Out.Pos = pos;

	Out.Tex = In.Tex;

	float3 nor = mul(In.Normal.xyz, (float4x3)g_WorldMatrix);
	Out.Normal.xyz = normalize(nor);
	float3 tan = mul(In.Tangent.xyz, (float4x3)g_WorldMatrix);
	Out.Tangent.xyz = normalize(tan);

	return Out;
}

/** 法線マップ計算. */
float3 NormalCalc(float3 normal,float3 tangent,float3 normalTexColor)
{
	float3 biNormal = normalize(cross(tangent, normal));
	normalTexColor = (normalTexColor * 2.0f) - 1.0f;
	return tangent * normalTexColor.x + biNormal * normalTexColor.y + normal * normalTexColor.z;
}

float3 SpecCalc(float3 normal,float3 worldPos)
{
	//スペキュラカラー.
	float3 SpecColor = 0;

	//頂点から視点へのベクトル.
	float3 toCameraPos = normalize(g_cameraPos.xyz - worldPos);

	float3 R = -toCameraPos + 2.0f * dot(normal, toCameraPos) * normal;

	for (int i = 0; i < g_LightNum; i++)
	{
		//反射ベクトル.
		float3 L = -g_diffuseLightDirection[i].xyz;

		//スペキュラ強度を計算.
		SpecColor += g_diffuseLightColor[i] * pow(max(0.0f, dot(L, R)), 2) * g_diffuseLightColor[i].w;
	}

	return SpecColor;
}

/**
* ピクセルシェーダ.
*/
float4 PSMain(VS_OUTPUT In) : COLOR0
{

	float4 defColor = 0;
	
	float3 color = float3(27, 61, 176) / 255;
	defColor = float4(color, 0.8f);

	//出力カラー.
	float4 OutColor = defColor;
	
	float3 normal = In.Normal.xyz;
	float3 tangent = In.Tangent.xyz;

	normal = NormalCalc(normal, tangent, tex2D(g_NormalSampler_1, In.Tex + float2(g_Wave, g_Wave)).xyz);
	normal = NormalCalc(normal, tangent, tex2D(g_NormalSampler_2, In.Tex + float2(g_Wave, -g_Wave)).xyz);
	
	float4 LightColor = DiffuseLight(normal);

	//スペキュラ.
	LightColor.xyz += SpecCalc(normal, In.WorldPos.xyz);

	LightColor.xyz += g_ambientLight.xyz * defColor.xyz;

	OutColor *= LightColor;

	return OutColor;
}

/**
* テクニック.
*/
technique Ocean
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}