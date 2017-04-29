/*!
 *@brief	3D���f���̕`��
 */
#include "LightingFunction.h"
bool Texflg;							//�e�N�X�`��
bool Spec;								//�X�y�L����
bool ReceiveShadow;						//�e���ʂ�

bool SkyBox;

float4x4 g_rotationMatrix;				//��]�s��B�@������]�����邽�߂ɕK�v�ɂȂ�B���C�e�B���O����Ȃ�K�{�B
float4x4 g_worldMatrix;					//���[���h�s��B
float4x4 g_viewMatrix;					//�r���[�s��B
float4x4 g_projectionMatrix;			//�v���W�F�N�V�����s��B

float4  g_diffuseMaterial;		//�}�e���A���J���[
float4  g_blendcolor;			//�S�̂ɍ�����F

float4x4 g_LVP;					//���C�g����݂��r���[�v���W�F�N�V�����s��

float2 g_TexelSize;				//�e�N�Z���T�C�Y(�[�x�o�b�t�@��)

float4  g_Textureblendcolor;	//�e�N�X�`���ɍ�����F
texture g_Texture;				//�e�N�X�`���B
sampler g_TextureSampler = 
sampler_state
{
	Texture = <g_Texture>;
    MipFilter = NONE;
    MinFilter = NONE;
    MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};
			
samplerCUBE g_cubeSampler =
sampler_state
{
	Texture = <g_Texture>;
	MipFilter = NONE;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture g_Shadow;				//�[�x�e�N�X�`��
sampler g_ShadowSampler_0 =
sampler_state
{
	Texture = <g_Shadow>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
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
	float4	_LVP	: TEXCOORD2;	//���C�g����݂�
};


/*!
 *@brief	���_�V�F�[�_�[�B
 */
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float4 pos;
	pos = mul( In._Pos, g_worldMatrix );		//���f���̃��[�J����Ԃ��烏�[���h��Ԃɕϊ��B
	
	Out._World = pos;						//���[���h���W
	Out._LVP = mul(pos, g_LVP);				//���C�g�̖ڐ��ɂ�郏�[���h�r���[�ˉe�ϊ�������
	//�X�J�C�{�b�N�X�̓r���[�s��������Ȃ��B
	if (!SkyBox)
	{
		pos = mul(pos, g_viewMatrix);			//���[���h��Ԃ���r���[��Ԃɕϊ��B
	}
	pos = mul( pos, g_projectionMatrix );	//�r���[��Ԃ���ˉe��Ԃɕϊ��B
	Out._World.w = pos.w;
	Out._Pos = pos;
	Out._Color = In._Color;
	Out._UV = In._UV;
	Out._Normal = mul(In._Normal, g_rotationMatrix);	//�@�����񂷁B
	
	return Out;
}

/*!
 *@brief	�s�N�Z���V�F�[�_�[�B
 */
float4 PSMain(VS_OUTPUT In):COLOR0
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
			return diff;
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

	if (g_EffectFlg.x)
	{
		sampler texSampler[3];
		texSampler[0] = g_ShadowSampler_0;
		texSampler[1] = g_ShadowSampler_0;
		texSampler[2] = g_ShadowSampler_0;

		for (int i = 0; i < 1; i++) {
			float4 posInLVP = In._LVP;
			posInLVP.xyz /= posInLVP.w;
			//uv���W�ɕϊ��B
			float2 shadowMapUV = float2(0.5f, -0.5f) * posInLVP.xy + float2(0.5f, 0.5f);
			float2 shadow_val = 1.0f;

			if (shadowMapUV.x < 0.99f && shadowMapUV.y < 0.99f && shadowMapUV.x > 0.01f && shadowMapUV.y > 0.01f) {
				shadow_val = tex2D(texSampler[i], shadowMapUV).rg;
				float depth = min(posInLVP.z, 1.0f);
				//�o���A���X�V���h�E�}�b�v�̃t���O
				if (false) {
					if (depth > shadow_val.r) {
						// ��^2
						float depth_sq = shadow_val.r * shadow_val.r;
						float variance = max(shadow_val.g - depth_sq, 0.0006f);
						float md = depth - shadow_val.r;
						float P = variance / (variance + md * md);
						light.rgb *= pow(P, 5.0f);
					}
				}
				else {
					if (depth > shadow_val.r + 0.006f) {
						//�F����
						light.rgb *= 0.5f;
					}
				}
				break;
			}
		}
	}

	//���C�g��������
	color.rgb *= light.rgb;
	//�A���r�G���g���C�g�����Z�B
	color.rgb += diff.rgb * g_ambientLight.rgb;
	
	return color;
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

float4 PSTerrain(VS_OUTPUT In) : COLOR
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
	////�e
	//light *= CalcShadow(In._World.xyz);
	color *= light;

	////��C�����B
	////color = In.rayColor + color * In.mieColor;

	////�|�C���g���C�g�B
	//color.xyz += diffuseColor.xyz * PointLight(normal, In._World.xyz, 0);
	////�A���r�G���g���C�g�����Z�B
	color.xyz += diffuseColor.xyz * g_light.ambient.xyz;

	//PSOutput psOut = (PSOutput)0;
	//psOut.velocity.xy = In.velocity.xy / In.velocity.w - In.screenPos.xy / In.screenPos.w;
	//psOut.velocity.xy *= 0.5f;
	//psOut.velocity.xy += 0.5f;
	//psOut.velocity.zw = 0.0f;

	//psOut.color = color;
	//psOut.depth = In._World.w;

	//return psOut;
	return color;
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
//���O�`��

struct PS_PreOUTPUT {
	float4	_Color	: COLOR0;		//�F
	float4	_Depth	: COLOR1;		//�[�x
	float4	_Luminance	: COLOR2;	//�P�x
};

PS_PreOUTPUT PSPre(VS_OUTPUT In)
{
	PS_PreOUTPUT o = (PS_PreOUTPUT)0;	//�ŏI�I�ɏo�͂���J���[
	float4 diff = (float4)0;			//���b�V���̃}�e���A��
	float4 color = (float4)0;
	float3 normal = normalize(In._Normal.xyz);
	//�J���[
	if (Texflg)
	{
		if (SkyBox)
		{

			//���]���Ă���̂�-1�������Ė@�������ǂ�
			o._Color = texCUBE(g_cubeSampler, In._Normal * -1.0f);
			return o;
		}
		else
		{
			diff = tex2D(g_TextureSampler, In._UV) * g_Textureblendcolor;
		}
	}
	else
	{
		//�}�e���A���̐F���̂܂�
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

	if (g_EffectFlg.x)
	{
		sampler texSampler[3];
		texSampler[0] = g_ShadowSampler_0;
		texSampler[1] = g_ShadowSampler_0;
		texSampler[2] = g_ShadowSampler_0;

		for (int i = 0; i < 1; i++) {
			float4 posInLVP = In._LVP;
			posInLVP.xyz /= posInLVP.w;
			//uv���W�ɕϊ��B
			float2 shadowMapUV = float2(0.5f, -0.5f) * posInLVP.xy + float2(0.5f, 0.5f);
			float2 shadow_val = 1.0f;

			if (shadowMapUV.x < 0.99f && shadowMapUV.y < 0.99f && shadowMapUV.x > 0.01f && shadowMapUV.y > 0.01f) {
				shadow_val = tex2D(texSampler[i], shadowMapUV).rg;
				float depth = min(posInLVP.z, 1.0f);
				//�o���A���X�V���h�E�}�b�v�̃t���O
				if (false) {
					if (depth > shadow_val.r) {
						// ��^2
						float depth_sq = shadow_val.r * shadow_val.r;
						float variance = max(shadow_val.g - depth_sq, 0.0006f);
						float md = depth - shadow_val.r;
						float P = variance / (variance + md * md);
						light.rgb *= pow(P, 5.0f);
					}
				}
				else {
					if (depth > shadow_val.r + 0.006f) {
						//�F����
						light.rgb *= 0.5f;
					}
				}
				break;
			}
		}
	}
	//�e�ɂȂ��Ă��Ȃ��B

	//���C�g��������
	color.rgb *= light.rgb;
	//�A���r�G���g���C�g�����Z�B
	color.rgb += diff.rgb * g_ambientLight.rgb;

	return o;
}

technique PreRender
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSPre();
	}
}

//////////////////////////////////////////////////////////////
//�e�`��p

struct VS_ShadowOUT {
	float4	_Pos		: POSITION;
	float4	_Shadow	: TEXCOORD;
};

VS_ShadowOUT VSShadow(float4 Pos : POSITION)
{
	VS_ShadowOUT Out = (VS_ShadowOUT)0;

	float4 pos;
	pos = mul(Pos, g_worldMatrix);		//���f���̃��[�J����Ԃ��烏�[���h��Ԃɕϊ��B
	pos = mul(pos, g_viewMatrix);		//���[���h��Ԃ���r���[��Ԃɕϊ��B
	pos = mul(pos, g_projectionMatrix);	//�r���[��Ԃ���ˉe��Ԃɕϊ��B

	Out._Shadow = Out._Pos = pos;

	return Out;
}

float4 PSShadow(VS_ShadowOUT In) : COLOR0	//�����_�[�^�[�Q�b�g0�ɏo��
{
	//�[�x
	float4 depth = (float4)0;

	//�[�x�͎ˉe�ϊ��ς݂̒��_�� Z / W �ŎZ�o�ł���
	depth = In._Shadow.z / In._Shadow.w;
	
	return float4(depth.xyz, 1.0f);
}

technique Shadow
{
	pass p0
	{
		VertexShader = compile vs_2_0 VSShadow();
		PixelShader = compile ps_2_0 PSShadow();
	}
}