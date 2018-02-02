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

struct VS_PUTPUT
{
	float4 Color : COLOR0;
	float4 Depth : COLOR1;
	float4 Luminance : COLOR2;
};

/*!
 * @brief	�����������p�̃s�N�Z���V�F�[�_�[�B
 */
VS_PUTPUT PSMainTrans( VS_OUTPUT In )
{
	VS_PUTPUT Out = (VS_PUTPUT)0;
	float4 tex = tex2D(g_textureSampler, In.uv);
	tex *= g_mulColor;

	Out.Color.xyz = tex.xyz * (1.0f + g_brightness);
	Out.Color.w = g_alpha  * tex.a;
	Out.Luminance = g_brightness;

	Out.Depth = float4(0, 0, 0, 0);

	return Out;
}
/*!
 * @brief	���Z�����p�̃s�N�Z���V�F�[�_�[�B
 */
VS_PUTPUT PSMainAdd( VS_OUTPUT In )
{
	VS_PUTPUT Out = (VS_PUTPUT)0;

	float4 tex = tex2D(g_textureSampler, In.uv);
	tex *= g_mulColor;

	Out.Color.xyz = tex.xyz * g_brightness * tex.a;
	Out.Color.w = g_alpha;
	Out.Luminance = g_brightness;
	
	Out.Depth = float4(0, 0, 0, 0);

	return Out;
}
/*!
 * @brief	�����������p�̃e�N�j�b�N�B
 */
technique ColorTexPrimTrans
{
	pass P0
    {          
        VertexShader = compile vs_3_0 VSMain();
        PixelShader  = compile ps_3_0 PSMainTrans();
    }
}
/*!
 * @brief	���Z�p�e�N�j�b�N�B
 */
technique ColorTexPrimAdd
{
    pass P0
    {          
        VertexShader = compile vs_3_0 VSMain();
        PixelShader  = compile ps_3_0 PSMainAdd();
    }
}