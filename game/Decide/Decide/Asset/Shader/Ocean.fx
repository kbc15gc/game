/**
* �C�V�F�[�_.
*/
#include"LightingFunction.h"

//�@���}�b�v.
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
//�@���}�b�v.
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

/** ���}�b�v�e�N�X�`��. */
textureCUBE g_EnvironmentMap;
samplerCUBE g_EnvironmentMapSampler = 
sampler_state
{
	Texture = <g_EnvironmentMap>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

/** ���[���h�s��. */
float4x4 g_WorldMatrix;
/** �r���[�s��. */
float4x4 g_ViewMatrix;
/** �v���W�F�N�V�����s��. */
float4x4 g_ProjMatrix;

/** �g. */
float g_Wave;

/**
* ���͒��_.
*/
struct VS_INPUT
{
	float4 Pos		: POSITION;		//!< ���_���W.
	float2 Tex		: TEXCOORD0;	//!< UV���W.
	float4 Normal	: NORMAL;		//!< �@���x�N�g��.
	float4 Tangent	: TANGENT;		//!< �ڃx�N�g��.
};

struct VS_OUTPUT
{
	float4 Pos		: POSITION;		//!< ���_���W.
	float2 Tex		: TEXCOORD0;	//!< UV���W.
    float4 Normal   : NORMAL; //!< �@���x�N�g��.
	float4 Tangent	: TEXCOORD1;	//!< �ڃx�N�g��.
	float4 WorldPos	: TEXCOORD2;	//!< ���[���h���W.
    float4 _MieColor : TEXCOORD3; //�~�[�����F�B
    float4 _RayColor : TEXCOORD4; //���C���[�����F�B
    float3 _PosToCameraDir : TEXCOORD5;
};

/**
* ���_�V�F�[�_.
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

    //��C����.
    CalcMieAndRayleighColors(Out._MieColor, Out._RayColor, Out._PosToCameraDir, Out.WorldPos.xyz);

	return Out;
}

/** �@���}�b�v�v�Z. */
float3 NormalCalc(float3 normal,float3 tangent,float3 normalTexColor)
{
	float3 biNormal = normalize(cross(tangent, normal));
	normalTexColor = (normalTexColor * 2.0f) - 1.0f;
	return tangent * normalTexColor.x + biNormal * normalTexColor.y + normal * normalTexColor.z;
}

float3 SpecCalc(float3 normal,float3 worldPos)
{
	//�X�y�L�����J���[.
	float3 SpecColor = 0;

	//���_���王�_�ւ̃x�N�g��.
	float3 toCameraPos = normalize(g_cameraPos.xyz - worldPos);

	float3 R = -toCameraPos + 2.0f * dot(normal, toCameraPos) * normal;

	for (int i = 0; i < g_LightNum; i++)
	{
		//���˃x�N�g��.
		float3 L = -g_diffuseLightDirection[i].xyz;

		//�X�y�L�������x���v�Z.
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
* �s�N�Z���V�F�[�_.
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

	//�o�̓J���[.
	float4 OutColor = defColor;

	{
		//���[���h��Ԃł̎����x�N�g��.
		float3 worldViewVec = In.WorldPos.xyz - g_cameraPos.xyz;
		float3 vReflect = reflect(worldViewVec, normal);
		//���}�b�v�̃J���[.
		float4 EnvironmentColor = texCUBE(g_EnvironmentMapSampler, vReflect);
		
		float R = 0.5f;
		OutColor.xyz = lerp(EnvironmentColor.xyz, defColor.xyz, R);

	}

	float4 LightColor = DiffuseLight(normal);

	//�X�y�L����.
	LightColor.xyz += SpecCalc(normal, In.WorldPos.xyz);

	OutColor *= LightColor;

    if (g_atmosFlag == AtomosphereFuncObjectFromAtomosphere)
    {
        OutColor.xyz = In._RayColor + OutColor * In._MieColor;
    }

	//����.
	OutColor.xyz += g_ambientLight.xyz * defColor.xyz;

	PS_OUTPUT Out = (PS_OUTPUT)0;

	Out.Color = OutColor;
	float3 depth = In.WorldPos.w;
	Out.Depth = float4(depth, 1.0f);

	return Out;
}

/**
* �e�N�j�b�N.
*/
technique Ocean
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}