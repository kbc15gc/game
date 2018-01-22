/**
* ScreenSpaceAmbientOcclusion.
* (�X�N���[���X�y�[�X�A���r�G���g�I�N�����[�W����)
* (��ʏ���ǁj
* ���x�ȃ��C�g���[�V���O���Z�ŁA
* ���ӌ����ǂ���͂��Ȃ����⃁�b�V���̌����A����A����ڂɈÂ�������A
* ���炩�ȑ��Ɩ��̉e���V�~�����[�g����B
*/

/**
* �J���[�e�N�X�`��.
*/
texture g_ColorTex;
sampler g_ColorTexSampler =
sampler_state
{
	Texture = <g_ColorTex>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

/**
* �[�x�e�N�X�`��.
*/
texture g_DepthTex;
sampler g_DepthTexSampler =
sampler_state
{
	Texture = <g_DepthTex>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

/**
* �@���e�N�X�`��.
*/
texture g_NormalTex;
sampler g_NormalTexSampler =
sampler_state
{
	Texture = <g_NormalTex>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

float2 g_TexSize;

//const float totStrength = 1.38;
const float totStrength = 2.00;
//const float strength = 0.0007;	//�[�x���Ɣ�r����l.�ō��l.
const float strength = 0.07;	//�[�x���Ɣ�r����l.�ō��l.
//const float falloff = 0.000002;	//�[�x���Ɣ�r����l.�Œ�l.
const float falloff = 0.0002;	//�[�x���Ɣ�r����l.�Œ�l.
//const float rad = 0.03;	//�[�x�Ŋ����Ă�.
const float rad = 0.03;	//�[�x�Ŋ����Ă�.

#define SAMPLES 16
const float invSamples = 1.0f / SAMPLES;

const float3 rayArray[SAMPLES] =
{
	float3(0.53812504f, 0.18565957f, -0.43192f),
	float3(0.13790712f, 0.24864247f, 0.44301823f),
	float3(0.33715037f, 0.56794053f, -0.005789503f),
	float3(-0.6999805f, -0.04511441f, -0.0019965635f),
	float3(0.06896307f, -0.15983082f, -0.85477847f),
	float3(0.056099437f, 0.006954967f, -0.1843352f),
	float3(-0.014653638f, 0.14027752f, 0.0762037f),
	float3(0.010019933f, -0.1924225f, -0.034443386f),
	float3(-0.35775623f, -0.5301969f, -0.43581226f),
	float3(-0.3169221f, 0.106360726f, 0.015860917f),
	float3(0.010350345f, -0.58698344f, 0.0046293875f),
	float3(-0.08972908f, -0.49408212f, 0.3287904f),
	float3(0.7119986f, -0.0154690035f, -0.09183723f),
	float3(-0.053382345f, 0.059675813f, -0.5411899f),
	float3(0.035267662f, -0.063188605f, 0.54602677f),
	float3(-0.47761092f, 0.2847911f, -0.0271716f),
};

struct VS_INPUT {
	float4	pos : POSITION;
	float2  uv : TEXCOORD;
};

struct VS_OUTPUT {
	float4 pos : POSITION;
	float2  uv : TEXCOORD;
};

/**
* ���_�V�F�[�_.
*/
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out;
	Out.pos = In.pos;		
	Out.uv = In.uv + float2(0.5f / g_TexSize.x, 0.5f / g_TexSize.y);
	return Out;
}

/**
* �s�N�Z���V�F�[�_.
*/
float4 PSSSAO(VS_OUTPUT In) : COLOR0
{
	//�@��.
	float3 normal = tex2D(g_NormalTexSampler, In.uv).xyz * 2.0f - 1.0f;
	//�[�x.
	float depth = tex2D(g_DepthTexSampler, In.uv);

	float bl = 0.0;
	float radD = rad / depth;

	float3 ray, occNorm;
	float2 se;
	float depthDifference, normDiff;

	for (int i = 0; i < SAMPLES; i++)
	{
		//�[�x�l�������ă��C�̕��������߂�.
		ray = radD * rayArray[i];
		se = In.uv + sign(dot(ray,normal)) * ray * float2(1.0f, -1.0f);
		occNorm = tex2D(g_NormalTexSampler, se.xy).xyz * 2.0f - 1.0f;
		//�[�x�����v�Z.
		depthDifference = depth - tex2D(g_DepthTexSampler, se.xy);
		normDiff = (1.0 - dot(normalize(occNorm), normalize(normal)));
		bl += step(falloff, depthDifference) * normDiff * (1.0 - smoothstep(falloff, strength, depthDifference));
	}

	//�A���r�G���g�I�N�����[�W�����l.
	float ao = 1.0 - (totStrength * bl * invSamples);

	float3 outColor = ao;//tex2D(g_ColorTexSampler, In.uv).xyz * ao;
	return float4(outColor, 1.0f);
}

technique RenderSSAO
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSSSAO();
	}
}