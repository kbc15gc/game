/*!
 *@brief	3D���f���̕`��
 */
#include"ShadowFunction.h"

#define MAX_LIGHTNUM 4
#include "LightingFunction.h"

int Texflg;							//�e�N�X�`��
int Spec; //�X�y�L����
int ReceiveShadow; //�e���ʂ�


int SkyBox;

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


texture g_NightTexture; //�e�N�X�`���B
samplerCUBE g_NightSampler =
sampler_state
{
    Texture = <g_NightTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//���_���\����

struct VS_INPUT
{
	float4	_Pos	: POSITION;
	float4	_Color	: COLOR0;
	float3	_Normal	: NORMAL;
    float3 _Tangent : TANGENT;
	float2	_UV		: TEXCOORD0;
};

struct VS_INPUT_INSTANCING {
	VS_INPUT base;
	float4 _World1	: TEXCOORD1;		//���[���h�s���1�s��
	float4 _World2	: TEXCOORD2;		//���[���h�s���2�s��
	float4 _World3	: TEXCOORD3;		//���[���h�s���3�s��
	float4 _World4	: TEXCOORD4;		//���[���h�s���4�s��
};

struct VS_OUTPUT
{
	float4	_Pos			: POSITION;
	float4	_Color			: COLOR0;
	float3	_Normal			: NORMAL;
    float3 _Tangent : TANGENT;
	float2	_UV				: TEXCOORD0;
	float4  _World			: TEXCOORD1;	//xyz�Ƀ��[���h���W�Bw�ɂ͎ˉe��Ԃł�depth���i�[�����B
	float4  _WVP			: TEXCOORD2;	//�J�������猩���s��
	float4	_MieColor		: TEXCOORD3;	//�~�[�����F�B
	float4	_RayColor		: TEXCOORD4;	//���C���[�����F�B
	float3  _PosToCameraDir	: TEXCOORD5;
};

/*!
 *@brief	���_�V�F�[�_�[�B
 */
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float4 pos;
	pos = mul(float4(In._Pos.xyz, 1.0f), g_worldMatrix);		//���f���̃��[�J����Ԃ��烏�[���h��Ԃɕϊ��B
	
	Out._World.xyz = pos.xyz;						//���[���h�s���ێ�

	//�X�J�C�{�b�N�X�̓r���[�s��������Ȃ��B
	//if (!SkyBox || g_isEnvironmentMap > 0.0f)
	{
		pos = mul(pos, g_viewMatrix);			//���[���h��Ԃ���r���[��Ԃɕϊ��B
	}
	pos = mul(pos, g_projectionMatrix );	//�r���[��Ԃ���ˉe��Ԃɕϊ��B

	Out._Pos = Out._WVP = pos;
	Out._World.w = pos.w;

	Out._Color = In._Color;
	Out._UV = In._UV;
    Out._Normal = normalize(mul(In._Normal, (float3x4) g_rotationMatrix)); //�@�����񂷁B
    Out._Tangent = normalize(mul(In._Tangent, (float3x4) g_rotationMatrix)); //�@�����񂷁B
	
	//��C�U��.
	CalcMieAndRayleighColors(Out._MieColor, Out._RayColor, Out._PosToCameraDir, Out._World.xyz);

	return Out;
}

VS_OUTPUT VSMainInstancing(VS_INPUT_INSTANCING In)
{
	VS_OUTPUT Out;

	float4x4 worldMat;
	worldMat[0] = In._World1;
	worldMat[1] = In._World2;
	worldMat[2] = In._World3;
	worldMat[3] = In._World4;

	float4 pos;
	pos = mul(float4(In.base._Pos.xyz, 1.0f), worldMat);		//���f���̃��[�J����Ԃ��烏�[���h��Ԃɕϊ��B

	Out._World.xyz = pos.xyz;						//���[���h�s���ێ�

	//�X�J�C�{�b�N�X�̓r���[�s��������Ȃ��B
	//if (!SkyBox || g_isEnvironmentMap > 0.0f)
	{
		pos = mul(pos, g_viewMatrix);			//���[���h��Ԃ���r���[��Ԃɕϊ��B
	}
	pos = mul(pos, g_projectionMatrix);	//�r���[��Ԃ���ˉe��Ԃɕϊ��B

	Out._Pos = Out._WVP = pos;
	Out._World.w = pos.w;

	Out._Color = In.base._Color;
	Out._UV = In.base._UV;
	float4x4 rotM;
	rotM[0] = In._World1;
	rotM[1] = In._World2;
	rotM[2] = In._World3;
    Out._Normal = normalize(mul(In.base._Normal, rotM)); //�@�����񂷁B
    Out._Tangent = normalize(mul(In.base._Tangent, rotM)); //�@�����񂷁B

	//��C�U��.
	CalcMieAndRayleighColors(Out._MieColor, Out._RayColor, Out._PosToCameraDir, Out._World.xyz);

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
	float4 color = 0.0f;	//�ŏI�I�ɏo�͂���J���[
	float4 diff = 0.0f;	//���b�V���̃}�e���A��

    //�@���v�Z.
    float3 normal = CalcNormal(In._Normal, In._Tangent, In._UV);

	//�J���[
	if (Texflg)
	{
		if(SkyBox)
		{
			
			//���]���Ă���̂�-1�������Ė@�������ǂ�
			diff = texCUBE(g_cubeSampler, normal * -1.0f);

			float3 mono = float3(0.29900f, 0.58700f, 0.11400f);

            //�����ɂ���.
			float Y = dot(mono, diff.xyz);

			//�����������e�N�X�`����n�悵�����ɋ߂��������������o���B
			float cloudRate = pow(Y, 3.0f);

            //����̔����v�f�𔲂��o��.
            float starRate = pow(dot(mono, texCUBE(g_NightSampler, normal * -1.0f).xyz), 3.0f);

			float nightRate = max(0.0f,dot(float3(0.0f,-1.0f,0.0f), g_atmosParam.v3LightDirection));

			//�_�̐F.
            float3 eyeToPos = normalize(In._World.xyz - g_cameraPos.xyz);
            float t = saturate(dot(eyeToPos, g_cameraDir));
			float cloudColor = lerp(0.1f, 1.0f, pow(1.0f - nightRate, 3.0f));
            float starColor = lerp(0.0f, 30.0f, pow(nightRate, 2.0f)) * starRate * pow(t, 10.0f);

            color = In._RayColor + 0.25f * In._MieColor;

			//��̐F.
			color.xyz = lerp(color.xyz, cloudColor, cloudRate);
            color.xyz += starColor;

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

    //��C�U��.
    if (g_atmosFlag == AtomosphereFuncObjectFromAtomosphere)
    {
        color.xyz = In._RayColor + color * In._MieColor;
    }

    float4 light = 0.0f;

    light.xyz += DiffuseLight(normal);
    light.xyz += CalcCharaLight(normal) * (float3(1.0f, 1.0f, 1.0f) - In._MieColor);
    
    if (Spec)
    {
        light.xyz += SpecLight(normal, In._World.xyz, In._UV);
        light.xyz += CalcCharaSpecLight(normal, In._World.xyz, In._UV);
    }
    
    if (g_EffectFlg.x)
    {
		//�e�ɂȂ��Ă���.
        float shadowPower = CalcShadow(In._World.xyz);
        shadowPower += (1.0f - max(0.0f, dot(g_atmosParam.v3LightDirection, float3(0.0f, 1.0f, 0.0f))));
        light.xyz *= min(1.0f, shadowPower);
    }

    color.xyz += diff.rgb * light.xyz;
    float3 ambient = g_ambientLight.rgb;
	
    if (g_CharaLightParam.x)
    {
        ambient += g_CharaLight.Ambient.rgb;
    }

    //�A���r�G���g���C�g�����Z�B
    color.rgb += diff.rgb * ambient;

	PSOutput Out = (PSOutput)0;

	Out.Color = color;
    clip(diff.a - g_Alpha); //@todo �A���t�@�e�X�g��臒l��萔���W�X�^�ő��肽���Ȃ� > �����N
	Out.Color.w = diff.a;
	float3 depth = In._World.w;
	Out.Depth = float4(depth, 1.0f);

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
//�C���X�^���V���O�`��p�B
technique InstancingRender
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainInstancing();
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
	float2 uv = In._UV;
	
	float4 diffuseColor = tex2D(g_terrainTexSampler[0], uv * 300.0f) * weights.x;
	diffuseColor += tex2D(g_terrainTexSampler[1], uv * 50.0f) * weights.y;
	diffuseColor += tex2D(g_terrainTexSampler[2], uv * 150.0f) * weights.z;
	//diffuseColor += tex2D(g_terrainTexSampler[3], uv) * weights.w;
	float4 color = diffuseColor;

	float3 normal = normalize(In._Normal);
	//�f�B�t���[�Y���C�g
	float4 light = DiffuseLight(normal);
    light.xyz += CalcCharaLight(normal);

	if (g_EffectFlg.x)
	{
		//�e�ɂȂ��Ă���.
		float shadowPower = CalcShadow(In._World.xyz);
		shadowPower += (1.0f - max(0.0f, dot(g_atmosParam.v3LightDirection, float3(0.0f, 1.0f, 0.0f))));
		light.xyz *= min(1.0f, shadowPower);
	}

	color *= light;

    //��C�U��.
    if (g_atmosFlag == AtomosphereFuncObjectFromAtomosphere)
    {
        color.xyz = In._RayColor + color * In._MieColor;
    }

    float3 ambient = g_ambientLight.rgb;
	
    if (g_CharaLightParam.x)
    {
        ambient += g_CharaLight.Ambient.rgb;
    }

    //�A���r�G���g���C�g�����Z�B
    color.rgb += diffuseColor.rgb * ambient;

	PSOutput Out = (PSOutput)0;

    Out.Color = color;
	float3 depth = In._World.w;
	Out.Depth = float4(depth, 1.0f);

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