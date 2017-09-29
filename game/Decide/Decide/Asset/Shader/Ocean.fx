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

/** 環境マップテクスチャ. */
textureCUBE g_EnvironmentMap;
samplerCUBE g_EnvironmentMapSampler = 
sampler_state
{
	Texture = <g_EnvironmentMap>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
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
    float4 Normal   : NORMAL; //!< 法線ベクトル.
	float4 Tangent	: TEXCOORD1;	//!< 接ベクトル.
	float4 WorldPos	: TEXCOORD2;	//!< ワールド座標.
    float4 _MieColor : TEXCOORD3; //ミー錯乱色。
    float4 _RayColor : TEXCOORD4; //レイリー錯乱色。
    float3 _PosToCameraDir : TEXCOORD5;
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

	Out.WorldPos.w = pos.w;

	Out.Tex = In.Tex;

	float3 nor = mul(In.Normal.xyz, (float3x3)g_WorldMatrix);
	Out.Normal.xyz = normalize(nor);
	float3 tan = mul(In.Tangent.xyz, (float3x3)g_WorldMatrix);
	Out.Tangent.xyz = normalize(tan);

    //大気錯乱.
    CalcMieAndRayleighColors(Out._MieColor, Out._RayColor, Out._PosToCameraDir, Out.WorldPos.xyz);

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

struct PS_OUTPUT
{
	float4 Color : COLOR0;
	float4 Depth : COLOR1;
};

/**
* ピクセルシェーダ.
*/
PS_OUTPUT PSMain(VS_OUTPUT In)
{
	float3 normal = In.Normal.xyz;
	float3 tangent = In.Tangent.xyz;
	normal = NormalCalc(normal, tangent, tex2D(g_NormalSampler_1, In.Tex + float2(g_Wave, g_Wave)).xyz);
	normal = NormalCalc(normal, tangent, tex2D(g_NormalSampler_2, In.Tex + float2(g_Wave, -g_Wave)).xyz);
	

	float4 defColor = 0;
	float3 color = float3(0, 151, 156) / 255;
	defColor = float4(color, 0.3f);

	//出力カラー.
	float4 OutColor = defColor;

	{
		//ワールド空間での視線ベクトル.
		float3 worldViewVec = In.WorldPos.xyz - g_cameraPos.xyz;
		float3 vReflect = reflect(worldViewVec, normal);
		//環境マップのカラー.
		float4 EnvironmentColor = texCUBE(g_EnvironmentMapSampler, vReflect);
		
		float R = 0.5f;
		OutColor.xyz = lerp(EnvironmentColor.xyz, defColor.xyz, R);

	}

	float4 LightColor = DiffuseLight(normal);

	//スペキュラ.
	LightColor.xyz += SpecCalc(normal, In.WorldPos.xyz);

	OutColor *= LightColor;

    if (g_atmosFlag == AtomosphereFuncObjectFromAtomosphere)
    {
        OutColor.xyz = In._RayColor + OutColor * In._MieColor;
    }

	//環境光.
	OutColor.xyz += g_ambientLight.xyz * defColor.xyz;

	PS_OUTPUT Out = (PS_OUTPUT)0;

	Out.Color = OutColor;
	float3 depth = In.WorldPos.w;
	Out.Depth = float4(depth, 1.0f);

	return Out;
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