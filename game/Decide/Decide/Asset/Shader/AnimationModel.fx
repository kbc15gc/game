/*!
 * @brief	スキンモデルシェーダー。(4ボーンスキニング)
 */
#include"ShadowFunction.h"

#define MAX_LIGHTNUM 4
#include "LightingFunction.h"

bool Texflg;							//テクスチャ
bool Spec;								//スペキュラ
bool ReceiveShadow;						//影を写す

//スキン行列。
#define MAX_MATRICES  50
float4x3    g_mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;

float4x4 g_worldMatrix;					//ワールド行列。
float4x4 g_viewMatrix;					//ビュー行列。
float4x4 g_projectionMatrix;			//プロジェクション行列。
float4x4 g_rotationMatrix;				//回転行列。法線を回転させるために必要になる。ライティングするなら必須。

float	g_numBone;			//骨の数。

float4  g_diffuseMaterial : COLOR0;					//マテリアルカラー
float4  g_blendcolor;//混ぜる色

float2 g_TexelSize;				//テクセルサイズ(深度バッファの)

float4  g_Textureblendcolor;	//テクスチャに混ぜる色
texture g_diffuseTexture;		//ディフューズテクスチャ。
sampler g_diffuseTextureSampler = 
sampler_state
{
	Texture = <g_diffuseTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Wrap;
	AddressV = Wrap;
};

texture g_Depth;				//メインカメラから見た深度テクスチャ
sampler g_DepthSampler =
sampler_state
{
	Texture = <g_Depth>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

/*!
 * @brief	入力頂点
 */
struct VS_INPUT
{
    float4  _Pos             : POSITION;
    float4  _BlendWeights    : BLENDWEIGHT;
    float4  _BlendIndices    : BLENDINDICES;
    float3  _Normal          : NORMAL;
    float3 _Tangent : TANGENT;
	float4	_Color			: COLOR0;
    float2  _UV            : TEXCOORD0;
};

/*!
 * @brief	出力頂点。
 */
struct VS_OUTPUT
{
	float4  _Pos    : POSITION;
	float4	_Color	: COLOR0;
    float3 _Normal : NORMAL;
    float3 _Tangent : TANGENT;
    float2 _UV : TEXCOORD1;
	float4  _World	: TEXCOORD2;	//ワールド座標
	float4  _WVP	: TEXCOORD3;	//カメラから見た行列
	float4	_MieColor		: TEXCOORD4;	//ミー錯乱色。
	float4	_RayColor		: TEXCOORD5;	//レイリー錯乱色。
	float3  _PosToCameraDir	: TEXCOORD6;
};

VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	
	//ブレンドするボーンのインデックス。
	int4 IndexVector = D3DCOLORtoUBYTE4(In._BlendIndices);
	
	//ブレンドレート。
	float BlendWeightsArray[4] = (float[4])In._BlendWeights;
    int   IndexArray[4]        = (int[4])IndexVector;
    float LastWeight = 0.0f;
    float3 pos = 0.0f;
    float3 normal = 0.0f;
    float3 tangent = 0.0f;
	//ボーン数ループ?
    for (int iBone = 0; iBone < g_numBone-1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        pos += mul(In._Pos, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        normal += mul(In._Normal, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        tangent += mul(In._Tangent, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 

    //？？
	pos += (mul(In._Pos, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
    normal += (mul(In._Normal, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
	tangent += (mul(In._Tangent, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
	
	o._World = float4(pos.xyz, 1.0f);

	//ワールド行列
	//_Pos = mul(float4(_Pos.xyz, 1.0f), g_worldMatrix);
	//ビュー行列
	pos = mul(float4(pos.xyz, 1.0f), g_viewMatrix);
	//プロジェクション行列
	o._WVP = o._Pos = mul(float4(pos.xyz, 1.0f), g_projectionMatrix);
	o._World.w = o._Pos.w;

	//法線
	//o._Normal = mul(normal, g_rotationMatrix);	//法線を回す。
	o._Normal = normal;

    o._UV = In._UV;
	o._Color = In._Color;
	o._Tangent = tangent;
	//大気散乱.
	CalcMieAndRayleighColors(o._MieColor, o._RayColor, o._PosToCameraDir, o._World.xyz);

	return o;
}

struct PSOutput
{
	float4 Color : COLOR0;
	float4 Depth : COLOR1;
};

/*!
 * @brief	ピクセルシェーダー。
 */
PSOutput PSMain( VS_OUTPUT In )
{
	float4 color = (float4)0;	//最終的に出力するカラー
	float4 diff = (float4)0;	//メッシュのマテリアル
    
    float3 normal = CalcNormal(In._Normal, In._Tangent, In._UV);

    //カラー
	if (Texflg)
	{
		diff = tex2D(g_diffuseTextureSampler, In._UV) * g_Textureblendcolor;
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
	if(Spec)
	{
        light.xyz += SpecLight(normal, In._World.xyz, In._UV);
        light.xyz += CalcCharaSpecLight(normal, In._World.xyz, In._UV, (float3x3) g_rotationMatrix);
    }

	float3 cascadeColor = 0;

	//影
	if (ReceiveShadow)
	{
		//影になっている.
		float shadowPower = CalcShadow(In._World.xyz, cascadeColor);
		shadowPower += (1.0f - max(0.0f, dot(g_atmosParam.v3LightDirection, float3(0.0f, 1.0f, 0.0f))));
		light.xyz *= min(1.0f, shadowPower);
	}

	//ライトをかける
	color *= light;

	//大気散乱.
	if (g_atmosFlag == AtomosphereFuncObjectFromAtomosphere)
	{
        color.xyz = In._RayColor + color * In._MieColor;
	}
	float3 charaLig = CalcCharaLight(normal, (float3x3) g_rotationMatrix) * (float3(1.0f, 1.0f, 1.0f) - In._MieColor);
	if(Spec)
	{
		charaLig.xyz += CalcCharaSpecLight(normal, In._World.xyz, In._UV, (float3x3) g_rotationMatrix);
	}
	color.xyz += diff.rgb * charaLig;
    float3 ambient = g_ambientLight.rgb;
	
    if (g_CharaLightParam.x)
    {
        ambient += g_CharaLight.Ambient.rgb;
    }

    //アンビエントライトを加算。
    color.rgb += diff.rgb * ambient;

	PSOutput Out = (PSOutput)0;

    Out.Color = color;
	float3 depth = In._World.w;
	Out.Depth = float4(depth, 1.0f);

	return Out;
}

//本描画
technique NormalRender
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}


//////////////////////////////////////////////////////////////
//影描画用

struct VS_ShadowIN {
	float4  pos             : POSITION;
	float4  _BlendWeights    : BLENDWEIGHT;
	float4  _BlendIndices    : BLENDINDICES;
};

struct VS_ShadowOUT {
	float4	pos		: POSITION;
	float4	shadow		: TEXCOORD0;
};

VS_ShadowOUT VSShadow(VS_ShadowIN In)
{
	VS_ShadowOUT Out = (VS_ShadowOUT)0;

	//ブレンドするボーンのインデックス。
	int4 IndexVector = D3DCOLORtoUBYTE4(In._BlendIndices);

	//ブレンドレート。
	float BlendWeightsArray[4] = (float[4])In._BlendWeights;
	int   IndexArray[4] = (int[4])IndexVector;
	float LastWeight = 0.0f;
	float3 pos = 0.0f;
	//ボーン数ループ?
	for (int iBone = 0; iBone < g_numBone - 1; iBone++)
	{
		LastWeight = LastWeight + BlendWeightsArray[iBone];

		pos += mul(In.pos, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
	}
	LastWeight = 1.0f - LastWeight;

	//各行列をかける
	pos += (mul(In.pos, g_mWorldMatrixArray[IndexArray[g_numBone - 1]]) * LastWeight);	//最後のやつ？
	pos = mul(float4(pos.xyz, 1.0f), g_viewMatrix);
	float4 P = mul(float4(pos.xyz, 1.0f), g_projectionMatrix);
	Out.pos = P;
	Out.shadow = P;

	return Out;
}

float4 PSShadow(VS_ShadowOUT In) : COLOR0	//レンダーターゲット0に出力
{
	//深度
	float depth = 0.0f;

	//深度は射影変換済みの頂点の Z / W で算出できる
	depth = In.shadow.z / In.shadow.w;

	float dx = ddx(depth);
	float dy = ddy(depth);

	float4 ret = (float4)0;
	ret.x = depth;
	ret.y = depth * depth + 0.25f * (dx * dx + dy * dy);
	ret.z = 0.0f;
	ret.w = 1.0f;
	return ret;
}

technique Shadow
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSShadow();
		PixelShader = compile ps_3_0 PSShadow();
	}
}