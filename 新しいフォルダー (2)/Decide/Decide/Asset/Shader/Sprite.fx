/*
 *2D�`��shader
 */
//�}�g���b�N�X
float4x4 g_WP;

//UV�̎n�_
float2 g_UVStart;
//UV�̒���
float2 g_UVLength;

//��_
float g_Pivotx;
float g_Pivoty;

//�u�����h�J���[
float4 g_BlendColor;
//���ߐF(��)
float4 g_ClipColor;

//�G�t�F�N�g�t���O	x�̓t�F�[�h
float4 g_EffectFlg;
//�����郉�C��
float g_FadeLine;

//�e�N�X�`���B
texture g_texture;
sampler g_textureSampler =
sampler_state
{
	Texture = <g_texture>;
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
	//Out��0�ŏ�����
	VS_OUTPUT Out = (VS_OUTPUT)0;
	//�X�N���[���ϊ���������O�Ƀs�{�b�g����
	Out.pos = In.pos - float4(g_Pivotx, g_Pivoty, 0, 0);
	//�X�N���[����ʂɕϊ�
	Out.pos = mul(Out.pos, g_WP);
	
	//UV����
	Out.uv = g_UVStart + (In.uv * g_UVLength);

	return Out;
}

/*!
* @brief	�����������p�̃s�N�Z���V�F�[�_�[�B
*/
float4 ps_main(VS_OUTPUT In) : COLOR0
{
	float4 color = tex2D(g_textureSampler, In.uv);
	
	//�����F�Ɠ����F�Ȃ�s�N�Z���L��
	if (g_ClipColor.r == color.r &&
		g_ClipColor.g == color.g &&
		g_ClipColor.b == color.b &&
		g_ClipColor.a == color.a)
	{
		clip(-1);
	}

	//�t�F�[�h�t���O�m�F
	if (g_EffectFlg.x)
	{
		color.a = g_FadeLine;
	}

	return color * g_BlendColor;
}

//sprite�`��e�N�j�b�N
technique SpriteTech
{
	//����������
	pass p0
	{
		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_2_0 ps_main();
	}
};