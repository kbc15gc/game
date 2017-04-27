/*!
 *@brief	3D���f���̕`��
 */
#include"ShadowFunction.h"

#define MAX_LIGHTNUM 4
bool Texflg;							//�e�N�X�`��
bool Spec;								//�X�y�L����
bool ReceiveShadow;						//�e���ʂ�

bool SkyBox;

float4 g_cameraPos;						//�J�����̈ʒu

float4x4 g_rotationMatrix;				//��]�s��B�@������]�����邽�߂ɕK�v�ɂȂ�B���C�e�B���O����Ȃ�K�{�B
float4x4 g_worldMatrix;					//���[���h�s��B
float4x4 g_viewMatrix;					//�r���[�s��B
float4x4 g_projectionMatrix;			//�v���W�F�N�V�����s��B

int g_LightNum;									//���C�g�̐�
float4	g_diffuseLightDirection[MAX_LIGHTNUM];	//�f�B�t���[�Y���C�g�̕����B
float4	g_diffuseLightColor[MAX_LIGHTNUM];		//�f�B�t���[�Y���C�g�̃J���[�B
float4	g_ambientLight;							//�����B

float4  g_diffuseMaterial;		//�}�e���A���J���[
float4  g_blendcolor;			//�S�̂ɍ�����F

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
	float4  _World	: TEXCOORD1;
};


/*!
 *@brief	���_�V�F�[�_�[�B
 */
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float4 pos;
	pos = mul( In._Pos, g_worldMatrix );		//���f���̃��[�J����Ԃ��烏�[���h��Ԃɕϊ��B
	
	Out._World = pos;						//���[���h�s���ێ�
	//�X�J�C�{�b�N�X�̓r���[�s��������Ȃ��B
	if (!SkyBox)
	{
		pos = mul(pos, g_viewMatrix);			//���[���h��Ԃ���r���[��Ԃɕϊ��B
	}
	pos = mul( pos, g_projectionMatrix );	//�r���[��Ԃ���ˉe��Ԃɕϊ��B

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
	for (int i = 0; i < g_LightNum; i++)
	{
		//0.0f�����Ȃ�0.0f��Ԃ�
		light.rgb += max(0.0f, -dot(normal, g_diffuseLightDirection[i].xyz)) * g_diffuseLightColor[i].rgb;
	}

	//�X�y�L�����[���C�g
	if (Spec)
	{
		float3 spec = 0.0f;
		float3 toEyeDir = normalize(g_cameraPos.xyz - In._World);
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

	if (ReceiveShadow)
	{
		//�e�ɂȂ��Ă���.
		light.rgb *= CalcShadow(In._World.xyz);
	}

	color.rgb *= light.rgb + g_ambientLight.rgb;
	
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
//���O�`��

struct PS_PreOUTPUT {
	float4	_Color	: COLOR0;		//�F
	float4	_Depth	: COLOR1;		//�[�x
	float4	_Luminance	: COLOR2;	//�P�x
};

PS_PreOUTPUT PSPre(VS_OUTPUT In)
{
	PS_PreOUTPUT o = (PS_PreOUTPUT)0;	//�ŏI�I�ɏo�͂���J���[
	float4 diff = (float4)0;	//���b�V���̃}�e���A��
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
	o._Color = diff;

	float4 light = (float4)0;

	//�f�t���[�Y���C�g���v�Z�B
	for (int i = 0; i < g_LightNum; i++)
	{
		//0.0f�����Ȃ�0.0f��Ԃ�
		light.rgb += max(0.0f, -dot(normal, g_diffuseLightDirection[i].xyz)) * g_diffuseLightColor[i].rgb;
	}

	//�X�y�L�����[���C�g
	if (Spec)
	{
		float3 spec = 0.0f;
		float3 toEyeDir = normalize(g_cameraPos.xyz - In._World);
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

	if (ReceiveShadow)
	{
		//�e�ɂȂ��Ă���.
		light.rgb *= CalcShadow(In._World.xyz);
	}
	//�e�ɂȂ��Ă��Ȃ��B

	o._Color.rgb *= light.rgb + g_ambientLight.rgb;

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
	float depth = 0.0f;

	//�[�x�͎ˉe�ϊ��ς݂̒��_�� Z / W �ŎZ�o�ł���
	depth = In._Shadow.z / In._Shadow.w;

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