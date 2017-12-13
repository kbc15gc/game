/*!
 * @brief	�X�L�����f���V�F�[�_�[�B(4�{�[���X�L�j���O)
 */
#include"ShadowFunction.h"

#define MAX_LIGHTNUM 4
#include "LightingFunction.h"

bool Texflg;							//�e�N�X�`��
bool Spec;								//�X�y�L����
bool ReceiveShadow;						//�e���ʂ�

//�X�L���s��B
#define MAX_MATRICES  50
float4x3    g_mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;

float4x4 g_worldMatrix;					//���[���h�s��B
float4x4 g_viewMatrix;					//�r���[�s��B
float4x4 g_projectionMatrix;			//�v���W�F�N�V�����s��B
float4x4 g_rotationMatrix;				//��]�s��B�@������]�����邽�߂ɕK�v�ɂȂ�B���C�e�B���O����Ȃ�K�{�B

float	g_numBone;			//���̐��B

float4  g_diffuseMaterial : COLOR0;					//�}�e���A���J���[
float4  g_blendcolor;//������F

float2 g_TexelSize;				//�e�N�Z���T�C�Y(�[�x�o�b�t�@��)

float4  g_Textureblendcolor;	//�e�N�X�`���ɍ�����F

texture g_diffuseTexture;		//�f�B�t���[�Y�e�N�X�`���B
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

texture g_Depth;				//���C���J�������猩���[�x�e�N�X�`��
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
 * @brief	���͒��_
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
 * @brief	�o�͒��_�B
 */
struct VS_OUTPUT
{
	float4  _Pos    : POSITION;
	float4	_Color	: COLOR0;
    float3 _Normal : NORMAL;
    float3 _Tangent : TANGENT;
    float2 _UV : TEXCOORD0;
	float4  _World	: TEXCOORD1;	//���[���h���W
	float4  _WVP	: TEXCOORD2;	//�J�������猩���s��
	float4	_MieColor		: TEXCOORD3;	//�~�[�����F�B
	float4	_RayColor		: TEXCOORD4;	//���C���[�����F�B
	float3  _PosToCameraDir	: TEXCOORD5;
};

VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	
	//�u�����h����{�[���̃C���f�b�N�X�B
	int4 IndexVector = D3DCOLORtoUBYTE4(In._BlendIndices);
	
	//�u�����h���[�g�B
	float BlendWeightsArray[4] = (float[4])In._BlendWeights;
    int   IndexArray[4]        = (int[4])IndexVector;
    float LastWeight = 0.0f;
    float3 pos = 0.0f;
    float3 normal = 0.0f;
    float3 tangent = 0.0f;
	//�{�[�������[�v?
    for (int iBone = 0; iBone < g_numBone-1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        pos += mul(In._Pos, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        normal += mul(In._Normal, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        tangent += mul(In._Tangent, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 

    //�H�H
	pos += (mul(In._Pos, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
    normal += (mul(In._Normal, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
	tangent += (mul(In._Tangent, g_mWorldMatrixArray[IndexArray[g_numBone-1]]) * LastWeight);
	
	o._World = float4(pos.xyz, 1.0f);

	//���[���h�s��
	//_Pos = mul(float4(_Pos.xyz, 1.0f), g_worldMatrix);
	//�r���[�s��
	pos = mul(float4(pos.xyz, 1.0f), g_viewMatrix);
	//�v���W�F�N�V�����s��
	o._WVP = o._Pos = mul(float4(pos.xyz, 1.0f), g_projectionMatrix);
	o._World.w = o._Pos.w;

	//�@��
	//o._Normal = mul(normal, g_rotationMatrix);	//�@�����񂷁B
    o._Normal = normalize(normal);

    o._UV = In._UV;
	o._Color = In._Color;
    o._Tangent = normalize(tangent);
	//��C�U��.
	CalcMieAndRayleighColors(o._MieColor, o._RayColor, o._PosToCameraDir, o._World.xyz);

	return o;
}

struct PSOutput
{
	float4 Color : COLOR0;
	float4 Depth : COLOR1;
	float4 Luminance : COLOR2;
};

/*!
 * @brief	�s�N�Z���V�F�[�_�[�B
 */
PSOutput PSMain( VS_OUTPUT In )
{
	CalcDither(In._WVP, In._World.xyz);

	float4 diff = (float4)0;	//���b�V���̃}�e���A��
    
    float3 normal = CalcNormal(In._Normal, In._Tangent, In._UV);

    //�J���[
	if (Texflg)
	{
		diff = tex2D(g_diffuseTextureSampler, In._UV) * g_Textureblendcolor;
	}
	else
	{
		diff = g_diffuseMaterial;
	}
	diff *= g_blendcolor;
	
    float4 color = diff; //�ŏI�I�ɏo�͂���J���[
	
    float4 light = DiffuseLight(normal);

    light.xyz += CalcCharaLight(normal) * (float3(1.0f, 1.0f, 1.0f) - In._MieColor);

    if (Spec)
    {
        light.xyz += SpecLight(normal, In._World.xyz, In._UV);
        light.xyz += CalcCharaSpecLight(normal, In._World.xyz, In._UV);
    }

    float shadowPower = 1.0f;
    //�e
    if (ReceiveShadow)
    {
		//�e�ɂȂ��Ă���.
        shadowPower = CalcShadow(In._World.xyz);
        shadowPower += (1.0f - abs(dot(g_atmosParam.v3LightDirection, float3(0.0f, 1.0f, 0.0f))));
        light.xyz *= min(1.0f, shadowPower);
    }

    color.xyz *= light.xyz;

    //��C�U��.
    if (g_atmosFlag == AtomosphereFuncObjectFromAtomosphere)
    {
        color.xyz = In._RayColor + color.xyz * In._MieColor;
    }

    color.xyz += diff.xyz * CalcMoonLight(normal, (float3) In._World, In._UV) * shadowPower;


    float3 ambient = g_ambientLight.rgb;
	
    ambient += g_CharaLight.Ambient.rgb;

    //�A���r�G���g���C�g�����Z�B
    color.rgb += diff.rgb * ambient;

	//�t�H�O���v�Z.
	color.xyz = CalcFog(In._World.xyz, color.xyz);

	PSOutput Out = (PSOutput)0;

    Out.Color.xyz = color.xyz;
    Out.Color.w = diff.a;
	float3 depth = In._World.w;
	Out.Depth = float4(depth, 1.0f);

	Out.Luminance = 0.0f;
	if (g_IsLuminance)
	{
		//�P�x���v�Z.
		float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
		Out.Luminance = max(0.0f, t - 1.0f);
	}

	return Out;
}

//�{�`��
technique NormalRender
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}


//////////////////////////////////////////////////////////////
//�e�`��p

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

	//�u�����h����{�[���̃C���f�b�N�X�B
	int4 IndexVector = D3DCOLORtoUBYTE4(In._BlendIndices);

	//�u�����h���[�g�B
	float BlendWeightsArray[4] = (float[4])In._BlendWeights;
	int   IndexArray[4] = (int[4])IndexVector;
	float LastWeight = 0.0f;
	float3 pos = 0.0f;
	//�{�[�������[�v?
	for (int iBone = 0; iBone < g_numBone - 1; iBone++)
	{
		LastWeight = LastWeight + BlendWeightsArray[iBone];

		pos += mul(In.pos, g_mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
	}
	LastWeight = 1.0f - LastWeight;

	//�e�s���������
	pos += (mul(In.pos, g_mWorldMatrixArray[IndexArray[g_numBone - 1]]) * LastWeight);	//�Ō�̂�H
	pos = mul(float4(pos.xyz, 1.0f), g_viewMatrix);
	float4 P = mul(float4(pos.xyz, 1.0f), g_projectionMatrix);
	Out.pos = P;
	Out.shadow = P;

	return Out;
}

float4 PSShadow(VS_ShadowOUT In) : COLOR0	//�����_�[�^�[�Q�b�g0�ɏo��
{
	//�[�x
	float depth = 0.0f;

	//�[�x�͎ˉe�ϊ��ς݂̒��_�� Z / W �ŎZ�o�ł���
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