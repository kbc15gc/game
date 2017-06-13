/*!
 *@brief	3D���f���̕`��
 */
#include"ShadowFunction.h"

#define MAX_LIGHTNUM 4
#include "LightingFunction.h"

bool Texflg;							//�e�N�X�`��
bool Spec;								//�X�y�L����
bool ReceiveShadow;						//�e���ʂ�

bool SkyBox;

/** ���}�b�v�t���O. */
int g_isEnvironmentMap;

float4x4 g_rotationMatrix;				//��]�s��B�@������]�����邽�߂ɕK�v�ɂȂ�B���C�e�B���O����Ȃ�K�{�B
float4x4 g_worldMatrix;					//���[���h�s��B
float4x4 g_viewMatrix;					//�r���[�s��B
float4x4 g_projectionMatrix;			//�v���W�F�N�V�����s��B

float4  g_diffuseMaterial;		//�}�e���A���J���[
float4  g_blendcolor;			//�S�̂ɍ�����F

float2 g_TexelSize;				//�e�N�Z���T�C�Y(�[�x�o�b�t�@��)

float4  g_Textureblendcolor;	//�e�N�X�`���ɍ�����F
texture g_Texture;				//�e�N�X�`���B
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

//���_���\����

struct VS_INPUT{
	float4	_Pos		: POSITION;
	float4	_Color	: COLOR0;
	float3	_Normal	: NORMAL;
	float2	_UV		: TEXCOORD0;
};

struct VS_OUTPUT{
	float4	_Pos	: POSITION;
	float4	_Color	: COLOR0;
	float3	_Normal	: NORMAL;
	float2	_UV		: TEXCOORD0;
	float4  _World	: TEXCOORD1;	//xyz�Ƀ��[���h���W�Bw�ɂ͎ˉe��Ԃł�depth���i�[�����B
	float4  _WVP	: TEXCOORD2;	//�J�������猩���s��
};


/*!
 *@brief	���_�V�F�[�_�[�B
 */
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float4 pos;
	pos = mul( float4(In._Pos.xyz,1.0f), g_worldMatrix );		//���f���̃��[�J����Ԃ��烏�[���h��Ԃɕϊ��B
	
	Out._World = pos;						//���[���h�s���ێ�

	//�X�J�C�{�b�N�X�̓r���[�s��������Ȃ��B
	if (!SkyBox || g_isEnvironmentMap > 0.0f)
	{
		pos = mul(pos, g_viewMatrix);			//���[���h��Ԃ���r���[��Ԃɕϊ��B
	}
	pos = mul(pos, g_projectionMatrix );	//�r���[��Ԃ���ˉe��Ԃɕϊ��B

	Out._Pos = Out._WVP = pos;
	Out._World.w = pos.w;

	Out._Color = In._Color;
	Out._UV = In._UV;
	Out._Normal = mul(In._Normal, g_rotationMatrix);	//�@�����񂷁B
	
	return Out;
}

struct PSOutput
{
	float4 Color : COLOR0;
	float4 Depth : COLOR1;
};

/*!
 *@brief	�s�N�Z���V�F�[�_�[�B
 */
PSOutput PSMain(VS_OUTPUT In)
{
	float4 color = (float4)0;	//�ŏI�I�ɏo�͂���J���[
	float4 diff = (float4)0;	//���b�V���̃}�e���A��
	float3 normal = normalize(In._Normal.xyz);
	//�J���[
	if (Texflg)
	{
		if(SkyBox)
		{
			
			//���]���Ă���̂�-1�������Ė@�������ǂ�
			diff = texCUBE(g_cubeSampler, In._Normal * -1.0f);

			PSOutput Out = (PSOutput)0;

			Out.Color = diff;
			Out.Depth = In._WVP.z / In._WVP.w;

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

	float4 light = (float4)0;

	//�f�t���[�Y���C�g���v�Z�B
	light = DiffuseLight(normal);

	//�X�y�L�����[���C�g
	if (Spec)
	{
		float3 spec = 0.0f;
		float3 toEyeDir = normalize(g_cameraPos.xyz - In._World.xyz);
		float3 R = -toEyeDir + 2.0f * dot(normal, toEyeDir) * normal;
		for (int i = 0; i < g_LightNum; i++)
		{
			//�X�y�L�����������v�Z����B
			//���˃x�N�g�����v�Z�B
			float3 L = -g_diffuseLightDirection[i].xyz;
			spec += g_diffuseLightColor[i] * pow(max(0.0f, dot(L, R)), 2) * g_diffuseLightColor[i].w;	//�X�y�L�������x�B
		}

		light.rgb += spec.xyz;
	}

	float3 cascadeColor = 0;

	if (g_EffectFlg.x)
	{
		//�e�ɂȂ��Ă���.
		light.rgb *= CalcShadow(In._World.xyz, cascadeColor);
	}

	//���C�g��������
	color.rgb *= light.rgb;
	//�A���r�G���g���C�g�����Z�B
	color.rgb += diff.rgb * g_ambientLight.rgb;
	
	//color.rgb *= cascadeColor;

	PSOutput Out = (PSOutput)0;

	Out.Color = color;
	Out.Depth = In._WVP.z / In._WVP.w;

	return Out;
}

//���ʂɕ`�悷��p
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
float4 g_terrainRect;	//!<�n�`��XZ���ʂŌ�����`�B

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
	//�X�v���b�g�}�b�v��UV���W�����߂�B
	float2 splatMapUV;
	splatMapUV.x = (In._World.x - g_terrainRect.x) / (g_terrainRect.y - g_terrainRect.x);
	splatMapUV.y = (In._World.z - g_terrainRect.z) / (g_terrainRect.w - g_terrainRect.z);
	float4 splatMap = tex2D(g_splatMapSampler, splatMapUV);
	//����g�������͎O���Ȃ̂�rgb�܂Ŏg��
	float t = splatMap.r + splatMap.g + splatMap.b /*+ splatMap.w*/;
	//�������v�Z
	float4 weights = float4(splatMap.r / t, splatMap.g / t, splatMap.b / t, splatMap.w / t);

	//�e�e�N�X�`������F���擾
	float4 diffuseColor = tex2D(g_terrainTexSampler[0], In._UV) * weights.x;
	diffuseColor += tex2D(g_terrainTexSampler[1], In._UV) * weights.y;
	diffuseColor += tex2D(g_terrainTexSampler[2], In._UV) * weights.z;
	//diffuseColor += tex2D(g_terrainTexSampler[3], In._UV) * weights.w;
	float4 color = diffuseColor;

	float3 normal = normalize(In._Normal);
	//�f�B�t���[�Y���C�g
	float4 light = DiffuseLight(normal);

	float3 cascadeColor = 1;

	if (g_EffectFlg.x)
	{
		//�e�ɂȂ��Ă���.
		light.rgb *= CalcShadow(In._World.xyz, cascadeColor);

	}

	color.rgb *= light.rgb;

	////�A���r�G���g���C�g�����Z�B
	color.xyz += diffuseColor.xyz * g_ambientLight.xyz;

	//color.xyz *= cascadeColor;

	PSOutput Out = (PSOutput)0;

	Out.Color = color;
	Out.Depth = In._WVP.z / In._WVP.w;

	return Out;
}

//���ʂɕ`�悷��p
technique TerrainRender
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSTerrain();
	}
}

//////////////////////////////////////////////////////////////
//�e�`��p

struct VS_ShadowOUT {
	float4	_Pos	: POSITION;
	float4	_Shadow	: TEXCOORD;
};

VS_ShadowOUT VSShadow(float4 Pos : POSITION)
{
	VS_ShadowOUT Out = (VS_ShadowOUT)0;

	float4 pos = Pos;
	pos = mul(pos, g_worldMatrix);		//���f���̃��[�J����Ԃ��烏�[���h��Ԃɕϊ��B
	pos = mul(pos, g_viewMatrix);		//���[���h��Ԃ���r���[��Ԃɕϊ��B
	pos = mul(pos, g_projectionMatrix);	//�r���[��Ԃ���ˉe��Ԃɕϊ��B
	Out._Shadow = Out._Pos = pos;

	return Out;
}

float4 PSShadow(VS_ShadowOUT In) : COLOR	//�����_�[�^�[�Q�b�g0�ɏo��
{
	//�[�x�͎ˉe�ϊ��ς݂̒��_�� Z / W �ŎZ�o�ł���
	//�[�x
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