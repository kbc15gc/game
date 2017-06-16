/*!
 *@brief	3Dモデルの描画
 */
#include"ShadowFunction.h"

#define MAX_LIGHTNUM 4
#include "LightingFunction.h"

bool Texflg;							//テクスチャ
bool Spec;								//スペキュラ
bool ReceiveShadow;						//影を写す

bool SkyBox;

/** 環境マップフラグ. */
int g_isEnvironmentMap;

float4x4 g_rotationMatrix;				//回転行列。法線を回転させるために必要になる。ライティングするなら必須。
float4x4 g_worldMatrix;					//ワールド行列。
float4x4 g_viewMatrix;					//ビュー行列。
float4x4 g_projectionMatrix;			//プロジェクション行列。

float4  g_diffuseMaterial;		//マテリアルカラー
float4  g_blendcolor;			//全体に混ぜる色

float2 g_TexelSize;				//テクセルサイズ(深度バッファの)

float4  g_Textureblendcolor;	//テクスチャに混ぜる色
texture g_Texture;				//テクスチャ。
sampler g_TextureSampler = 
sampler_state
{
	Texture = <g_Texture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
			
samplerCUBE g_cubeSampler =
sampler_state
{
	Texture = <g_Texture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

//頂点情報構造体

struct VS_INPUT
{
	float4	_Pos		: POSITION;
	float4	_Color	: COLOR0;
	float3	_Normal	: NORMAL;
	float2	_UV		: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4	_Pos			: POSITION;
	float4	_Color			: COLOR0;
	float3	_Normal			: NORMAL;
	float2	_UV				: TEXCOORD0;
	float4  _World			: TEXCOORD1;	//xyzにワールド座標。wには射影空間でのdepthが格納される。
	float4  _WVP			: TEXCOORD2;	//カメラから見た行列
	float4	_MieColor		: TEXCOORD3;	//ミー錯乱色。
	float4	_RayColor		: TEXCOORD4;	//レイリー錯乱色。
	float3  _PosToCameraDir	: TEXCOORD5;
};

/*!
 *@brief	頂点シェーダー。
 */
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float4 pos;
	pos = mul(float4(In._Pos.xyz, 1.0f), g_worldMatrix);		//モデルのローカル空間からワールド空間に変換。
	
	Out._World.xyz = pos.xyz;						//ワールド行列を保持

	//スカイボックスはビュー行列をかけない。
	//if (!SkyBox || g_isEnvironmentMap > 0.0f)
	{
		pos = mul(pos, g_viewMatrix);			//ワールド空間からビュー空間に変換。
	}
	pos = mul(pos, g_projectionMatrix );	//ビュー空間から射影空間に変換。

	Out._Pos = Out._WVP = pos;
	Out._World.w = pos.w;

	Out._Color = In._Color;
	Out._UV = In._UV;
	Out._Normal = mul(In._Normal, g_rotationMatrix);	//法線を回す。
	
	//大気散乱.
	CalcMieAndRayleighColors(Out._MieColor, Out._RayColor, Out._PosToCameraDir, Out._World.xyz);

	return Out;
}

struct PSOutput
{
	float4 Color : COLOR0;
	float4 Depth : COLOR1;
};

/*!
 *@brief	ピクセルシェーダー。
 */
PSOutput PSMain(VS_OUTPUT In)
{
	float4 color = 0.0f;	//最終的に出力するカラー
	float4 diff = 0.0f;	//メッシュのマテリアル
	float3 normal = normalize(In._Normal.xyz);
	//カラー
	if (Texflg)
	{
		if(SkyBox)
		{
			
			//反転しているので-1をかけて法線をもどす
			diff = texCUBE(g_cubeSampler, normal * -1.0f);

			//float3 mono = float3(0.29900f, 0.58700f, 0.11400f);

			//float Y = dot(mono, diff.xyz);

			//白黒化したテクスチャをn乗した白に近い成分だけ抜き出す。
			//float cloudRate = pow(Y, 3.0f);

			color = In._RayColor + 0.25f * In._MieColor;

			//大気の色もモノクロ化
			//float colorY = max(0.0f, dot(mono, color.xyz));

			//float nightRate = max(0.0f,dot(float3(0.0f,1.0f,0.0f), g_atmosParam.v3LightDirection));

			//雲の色.
			//float cloudColor = lerp(3.0f, 0.1f, pow(1.0f - nightRate, 3.0f));

			//空の色.
			//color.xyz = lerp(color.xyz, cloudColor, cloudRate);

			//color.xyz *= 1.1f;

			color.w = 1.0f;

			PSOutput Out = (PSOutput)0;

			Out.Color = color;
			float3 depth = In._World.w;
			Out.Depth = float4(depth, 1.0f);

			return Out;
		}
		else
		{
			diff = tex2D(g_TextureSampler, In._UV) * g_Textureblendcolor;
		}
	}
	else
	{
		diff = g_diffuseMaterial;
	}

	diff *= g_blendcolor;
	color = diff;

	float4 light = 0.0f;

	//デフューズライトを計算。
	light = DiffuseLight(normal);

	//スペキュラーライト
	if (Spec)
	{
		float3 spec = 0.0f;
		float3 toEyeDir = normalize(g_cameraPos.xyz - In._World.xyz);
		float3 R = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;
		for (int i = 0; i < g_LightNum; i++)
		{
			//スペキュラ成分を計算する。
			//反射ベクトルを計算。
			float3 L = -g_diffuseLightDirection[i].xyz;
			spec += g_diffuseLightColor[i] * pow(max(0.0f, dot(L, R)), 2) * g_diffuseLightColor[i].w;	//スペキュラ強度。
		}

		light.rgb += spec.xyz;
	}

	float3 cascadeColor = 0;

	if (g_EffectFlg.x)
	{
		//影になっている.
		light.xyz *= CalcShadow(In._World.xyz, cascadeColor);
	}

	//ライトをかける
	color *= light;

	//大気散乱.
	if (g_atmosFlag == AtomosphereFuncObjectFromAtomosphere)
	{
		color = In._RayColor + color * In._MieColor;
		color.w = diff.w;
	}

	//アンビエントライトを加算。
	color.rgb += diff.rgb * g_ambientLight.rgb;


	PSOutput Out = (PSOutput)0;

	Out.Color = color;
	float3 depth = In._World.w;
	Out.Depth = float4(depth, 1.0f);

	return Out;
}

//普通に描画する用
technique NormalRender
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}

//////////////////////////////////////////////////////////////
texture g_splatMap;			//Splatmap
sampler g_splatMapSampler =
sampler_state
{
	Texture = <g_splatMap>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
texture g_terrainTex0;
texture g_terrainTex1;
texture g_terrainTex2;
texture g_terrainTex3;
float4 g_terrainRect;	//!<地形をXZ平面で見た矩形。

sampler g_terrainTexSampler[4] = {
	sampler_state
{
	Texture = <g_terrainTex0>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
},
sampler_state
{
	Texture = <g_terrainTex1>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
},
sampler_state
{
	Texture = <g_terrainTex2>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
},
sampler_state
{
	Texture = <g_terrainTex3>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
},
};

texture g_terrainNormalMap0;
texture g_terrainNormalMap1;
texture g_terrainNormalMap2;
texture g_terrainNormalMap3;
sampler g_terrainNormalMapSampler[4] = {
	sampler_state
{
	Texture = <g_terrainNormalMap0>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
},
sampler_state
{
	Texture = <g_terrainNormalMap1>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
},
sampler_state
{
	Texture = <g_terrainNormalMap2>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
},
sampler_state
{
	Texture = <g_terrainNormalMap3>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
},
};

PSOutput PSTerrain(VS_OUTPUT In)
{
	//スプラットマップのUV座標を求める。
	float2 splatMapUV;
	splatMapUV.x = (In._World.x - g_terrainRect.x) / (g_terrainRect.y - g_terrainRect.x);
	splatMapUV.y = (In._World.z - g_terrainRect.z) / (g_terrainRect.w - g_terrainRect.z);
	float4 splatMap = tex2D(g_splatMapSampler, splatMapUV);
	//今回使う枚数は三枚なのでrgbまで使う
	float t = splatMap.r + splatMap.g + splatMap.b /*+ splatMap.w*/;
	//割合を計算
	float4 weights = float4(splatMap.r / t, splatMap.g / t, splatMap.b / t, splatMap.w / t);

	//各テクスチャから色を取得
	float4 diffuseColor = tex2D(g_terrainTexSampler[0], In._UV) * weights.x;
	diffuseColor += tex2D(g_terrainTexSampler[1], In._UV) * weights.y;
	diffuseColor += tex2D(g_terrainTexSampler[2], In._UV) * weights.z;
	//diffuseColor += tex2D(g_terrainTexSampler[3], In._UV) * weights.w;
	float4 color = diffuseColor;

	float3 normal = normalize(In._Normal);
	//ディフューズライト
	float4 light = DiffuseLight(normal);

	float3 cascadeColor = 1;

	if (g_EffectFlg.x)
	{
		//影になっている.
		light.xyz *= CalcShadow(In._World.xyz, cascadeColor);

	}
	
	color *= light;

	//大気散乱.
	if (g_atmosFlag == AtomosphereFuncObjectFromAtomosphere)
	{
		color = In._RayColor + color * In._MieColor;
		color.w = diffuseColor.w;
	}

	//アンビエントライトを加算。
	color.xyz += diffuseColor.xyz * g_ambientLight.xyz;

	PSOutput Out = (PSOutput)0;

	Out.Color = color;
	float3 depth = In._World.w;
	Out.Depth = float4(depth, 1.0f);

	return Out;
}

//普通に描画する用
technique TerrainRender
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSTerrain();
	}
}

//////////////////////////////////////////////////////////////
//影描画用

struct VS_ShadowOUT {
	float4	_Pos	: POSITION;
	float4	_Shadow	: TEXCOORD;
};

VS_ShadowOUT VSShadow(float4 Pos : POSITION)
{
	VS_ShadowOUT Out = (VS_ShadowOUT)0;

	float4 pos = Pos;
	pos = mul(pos, g_worldMatrix);		//モデルのローカル空間からワールド空間に変換。
	pos = mul(pos, g_viewMatrix);		//ワールド空間からビュー空間に変換。
	pos = mul(pos, g_projectionMatrix);	//ビュー空間から射影空間に変換。
	Out._Shadow = Out._Pos = pos;

	return Out;
}

float4 PSShadow(VS_ShadowOUT In) : COLOR	//レンダーターゲット0に出力
{
	//深度は射影変換済みの頂点の Z / W で算出できる
	//深度
	float z = In._Shadow.z / In._Shadow.w;

	float dx = ddx(z);
	float dy = ddy(z);

	return float4(z, z * z + 0.25f * (dx * dx + dy * dy), 0.0f, 1.0f);
}

technique Shadow
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSShadow();
		PixelShader = compile ps_3_0 PSShadow();
	}
}