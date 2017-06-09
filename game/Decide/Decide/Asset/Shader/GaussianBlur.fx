/**
* ガウシアンブラー.
*/

float2 g_offset;				//オフセット
float2 g_texelOffset;

float2 g_texSize;			//テクスチャサイズ。
float  g_weight[8];				//ガウスフィルタの重み。

texture g_Blur;
sampler TextureSampler =
sampler_state
{
	Texture = <g_Blur>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

struct VS_INPUT {
	float4		pos		: POSITION;
	float2		uv		: TEXCOORD0;
};
struct VS_OUTPUT_GBLUR {
	float4 pos	: POSITION;
	float2 tex0	: TEXCOORD0;
	float2 tex1 : TEXCOORD1;
	float2 tex2 : TEXCOORD2;
	float2 tex3 : TEXCOORD3;
	float2 tex4 : TEXCOORD4;
	float2 tex5 : TEXCOORD5;
	float2 tex6 : TEXCOORD6;
	float2 tex7 : TEXCOORD7;
};

/*!
* @brief	Xガウシアンブラーの頂点シェーダー。
*/
VS_OUTPUT_GBLUR VSMainGBlurX(VS_INPUT In)
{
	VS_OUTPUT_GBLUR Out = (VS_OUTPUT_GBLUR)0;
	Out.pos = In.pos;
	float2 tex = (In.pos * 0.5f) + 0.5f;
	tex.y = 1.0f - tex.y;
	tex += g_texelOffset;
	Out.tex0 = tex;
	Out.tex1 = tex + float2(-2.0f / g_texSize.x, 0.0f);
	Out.tex2 = tex + float2(-4.0f / g_texSize.x, 0.0f);
	Out.tex3 = tex + float2(-6.0f / g_texSize.x, 0.0f);
	Out.tex4 = tex + float2(-8.0f / g_texSize.x, 0.0f);
	Out.tex5 = tex + float2(-10.0f / g_texSize.x, 0.0f);
	Out.tex6 = tex + float2(-12.0f / g_texSize.x, 0.0f);
	Out.tex7 = tex + float2(-14.0f / g_texSize.x, 0.0f);

	return Out;
}

/*!
* @brief	Yガウシアンブラーの頂点シェーダー。
*/
VS_OUTPUT_GBLUR VSMainGBlurY(VS_INPUT In)
{
	VS_OUTPUT_GBLUR Out = (VS_OUTPUT_GBLUR)0;
	Out.pos = In.pos;
	float2 tex = (In.pos * 0.5f) + 0.5f;
	tex.y = 1.0f - tex.y;
	tex += g_texelOffset;
	Out.tex0 = tex;
	Out.tex1 = tex + float2(0.0f, -2.0f / g_texSize.y);
	Out.tex2 = tex + float2(0.0f, -4.0f / g_texSize.y);
	Out.tex3 = tex + float2(0.0f, -6.0f / g_texSize.y);
	Out.tex4 = tex + float2(0.0f, -8.0f / g_texSize.y);
	Out.tex5 = tex + float2(0.0f, -10.0f / g_texSize.y);
	Out.tex6 = tex + float2(0.0f, -12.0f / g_texSize.y);
	Out.tex7 = tex + float2(0.0f, -14.0f / g_texSize.y);
	return Out;

}
/*!
* @brief	ガウシアンブラーのピクセルシェーダー。
*/
float4 PSMainGBlur(VS_OUTPUT_GBLUR In, uniform int numWeight) : COLOR0
{
	float4 Color;
Color = g_weight[0] * tex2D(TextureSampler, In.tex0);
Color += g_weight[1] * (tex2D(TextureSampler, In.tex1) + tex2D(TextureSampler, In.tex7 + g_offset));
Color += g_weight[2] * (tex2D(TextureSampler, In.tex2) + tex2D(TextureSampler, In.tex6 + g_offset));
Color += g_weight[3] * (tex2D(TextureSampler, In.tex3) + tex2D(TextureSampler, In.tex5 + g_offset));
Color += g_weight[4] * (tex2D(TextureSampler, In.tex4) + tex2D(TextureSampler, In.tex4 + g_offset));
Color += g_weight[5] * (tex2D(TextureSampler, In.tex5) + tex2D(TextureSampler, In.tex3 + g_offset));
Color += g_weight[6] * (tex2D(TextureSampler, In.tex6) + tex2D(TextureSampler, In.tex2 + g_offset));
Color += g_weight[7] * (tex2D(TextureSampler, In.tex7) + tex2D(TextureSampler, In.tex1 + g_offset));
return Color;
}


/*!
* @brief	横ガウシアンブラー。ウェイト8
*/
technique TransformedPrimGBlurX_8
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainGBlurX();
		PixelShader = compile ps_3_0 PSMainGBlur(8);
	}
}

/*!
* @brief	縦ガウシアンブラー。ウェイト8
*/
technique TransformedPrimGBlurY_8
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainGBlurY();
		PixelShader = compile ps_3_0 PSMainGBlur(8);
	}
}

/*!
* @brief	横ガウシアンブラー。ウェイト6
*/
technique TransformedPrimGBlurX_6
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainGBlurX();
		PixelShader = compile ps_3_0 PSMainGBlur(6);
	}
}

/*!
* @brief	縦ガウシアンブラー。ウェイト8
*/
technique TransformedPrimGBlurY_6
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainGBlurY();
		PixelShader = compile ps_3_0 PSMainGBlur(6);
	}
}

/*!
* @brief	横ガウシアンブラー。ウェイト4
*/
technique TransformedPrimGBlurX_4
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainGBlurX();
		PixelShader = compile ps_3_0 PSMainGBlur(4);
	}
}

/*!
* @brief	縦ガウシアンブラー。ウェイト4
*/
technique TransformedPrimGBlurY_4
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainGBlurY();
		PixelShader = compile ps_3_0 PSMainGBlur(4);
	}
}

/*!
* @brief	横ガウシアンブラー。ウェイト2
*/
technique TransformedPrimGBlurX_2
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainGBlurX();
		PixelShader = compile ps_3_0 PSMainGBlur(2);
	}
}

/*!
* @brief	縦ガウシアンブラー。ウェイト2
*/
technique TransformedPrimGBlurY_2
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMainGBlurY();
		PixelShader = compile ps_3_0 PSMainGBlur(2);
	}
}

