/**
* �C�V�F�[�_.
*/

/** �e�N�X�`��. */
texture g_Texture;		
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

/** ���[���h�s��. */
float4x4 g_WorldMatrix;
/** �r���[�s��. */
float4x4 g_ViewMatrix;
/** �v���W�F�N�V�����s��. */
float4x4 g_ProjMatrix;

/**
* ���͒��_.
*/
struct VS_INPUT
{
	float4 Pos	: POSITION;		//!< ���_���W.
	float2 Tex	: TEXCOORD0;	//!< UV���W.
};

struct VS_OUTPUT
{
	float4 Pos	: POSITION;		//!< ���_���W.
	float2 Tex	: TEXCOORD0;	//!< UV���W.
};

/**
* ���_�V�F�[�_.
*/
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	float4 pos = In.Pos;

	pos = mul(pos, g_WorldMatrix);
	pos = mul(pos, g_ViewMatrix);
	pos = mul(pos, g_ProjMatrix);

	Out.Pos = pos;
	Out.Tex = In.Tex;

	return Out;
}

/**
* �s�N�Z���V�F�[�_.
*/
float4 PSMain(VS_OUTPUT In) : COLOR0
{
	float4 color = tex2D(g_TextureSampler, In.Tex);
	return color + float4(0.0f,0.0f,1.0f,1.0f);
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