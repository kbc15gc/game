/*
 *2D�`��shader
 */

//�t�F�[�h�J���[
float4 g_FadeColor;

//�����郉�C��
float g_Alpha;

//�e�N�X�`�����g�p���邩�ǂ����H
bool g_UseTexture;

//�e�N�X�`���B
texture g_Texture;
sampler g_TextureSampler =
sampler_state
{
	Texture = <g_Texture>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT
{
	float4 pos		: POSITION;
	float2 uv		: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 pos		: POSITION;
	float2 uv		: TEXCOORD0;
};

//���_�V�F�[�_
VS_OUTPUT vs_main(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	//���K������Ă���̂ł��̂܂�
	Out.pos = In.pos;
	//���̂܂�
	Out.uv = In.uv;

	return Out;
}

/*!
* @brief	�����������p�̃s�N�Z���V�F�[�_�[�B
*/
float4 ps_main(VS_OUTPUT In) : COLOR0
{
	//�e�N�X�`������J���[�擾
	float4 color = float4(1,1,1,1);
	if (g_UseTexture)
		color = tex2D(g_TextureSampler, In.uv);
	else
		color = g_FadeColor;
	//�A���t�@�ύX
	color.a = g_Alpha;

	return color;
}

technique SceneTech
{
	pass p0
	{
		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_2_0 ps_main();
	}
};