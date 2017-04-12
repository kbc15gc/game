/*!
 *@brief	�J���[�e�N�X�`���v���~�e�B�u�B
 */
float4x4	g_mWVP;		//���[���h�r���[�v���W�F�N�V�����s��B

float4 g_mulColor;
float g_brightness;
float g_alpha;			//!<�A���t�@
texture g_texture;		//�f�B�t���[�Y�e�N�X�`���B
sampler g_textureSampler =
sampler_state
{
	Texture = <g_texture>;
	MipFilter = NONE;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT{
	float4	pos		: POSITION;
	float2	uv		: TEXCOORD0;
};

struct VS_OUTPUT{
	float4 pos		: POSITION;
	float2	uv		: TEXCOORD0;
};

VS_OUTPUT VSMain( VS_INPUT In )
{
	//Out��0�ŏ�����
	VS_OUTPUT Out = (VS_OUTPUT)0;
	//���W�𒆐S��
	Out.pos = In.pos /*- float4(0.5f, 0.5f, 0.0f, 0.0f)*/;

	Out.pos = mul(Out.pos, g_mWVP);
	//uv����
	Out.uv = In.uv;
	return Out;
}
/*!
 * @brief	�����������p�̃s�N�Z���V�F�[�_�[�B
 */
float4 PSMainTrans( VS_OUTPUT In ) : COLOR0
{
	float4 tex = tex2D(g_textureSampler, In.uv);
	tex *= g_mulColor;
	return float4(tex.xyz, tex.a * g_alpha );
}
/*!
 * @brief	���Z�����p�̃s�N�Z���V�F�[�_�[�B
 */
float4 PSMainAdd( VS_OUTPUT In ) : COLOR0
{
	float4 tex = tex2D(g_textureSampler, In.uv);
	tex *= g_mulColor;
	return float4(tex.xyz * g_alpha, tex.a/g_brightness );
}
/*!
 * @brief	�����������p�̃e�N�j�b�N�B
 */
technique ColorTexPrimTrans
{
	pass P0
    {          
        VertexShader = compile vs_2_0 VSMain();
        PixelShader  = compile ps_2_0 PSMainTrans();
    }
}
/*!
 * @brief	���Z�p�e�N�j�b�N�B
 */
technique ColorTexPrimAdd
{
    pass P0
    {          
        VertexShader = compile vs_2_0 VSMain();
        PixelShader  = compile ps_2_0 PSMainAdd();
    }
}