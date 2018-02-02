/*!
 *@brief	カラーテクスチャプリミティブ。
 */
float4x4	g_mWVP;		//ワールドビュープロジェクション行列。

float4 g_mulColor;
float g_brightness;
float g_alpha;			//!<アルファ
texture g_texture;		//ディフューズテクスチャ。
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
	//Outを0で初期化
	VS_OUTPUT Out = (VS_OUTPUT)0;
	//座標を中心に
	Out.pos = In.pos /*- float4(0.5f, 0.5f, 0.0f, 0.0f)*/;

	Out.pos = mul(Out.pos, g_mWVP);
	//uv調整
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
 * @brief	半透明合成用のピクセルシェーダー。
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
 * @brief	加算合成用のピクセルシェーダー。
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
 * @brief	半透明合成用のテクニック。
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
 * @brief	加算用テクニック。
 */
technique ColorTexPrimAdd
{
    pass P0
    {          
        VertexShader = compile vs_3_0 VSMain();
        PixelShader  = compile ps_3_0 PSMainAdd();
    }
}