/**
* 被写界深度のシェーダ.
*/

#define USE_BLOOM_FLOATING_BUFFER	//定義でブルームで浮動小数点バッファを使用する。

/** シーンテクスチャサイズ. */
float2 g_SceneTexSize;

/** x:手前ボケ、y:奥ボケ、z:ピント. */
float3 g_DofParam;

/** 深度テクスチャ. */
texture g_Depth;
sampler g_DepthSampler =
sampler_state
{
	Texture = <g_Depth>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

/** 奥ボケテクスチャ. */
texture g_BlurBack;
sampler g_BlurBackSampler =
sampler_state
{
	Texture = <g_BlurBack>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

texture g_BlurForward;	//手前ボケ
sampler g_BlurForwardSampler =
sampler_state
{
	Texture = <g_BlurForward>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


/**
* 頂点シェーダ入力. 
*/
struct VS_INPUT_COMBINE
{
	float4	Pos : POSITION;
};

/**
* 頂点シェーダ出力.
*/
struct VS_OUTPUT 
{
	float4 Pos 	: POSITION;
	float2 Tex	: TEXCOORD0;
};

/**
* 頂点シェーダ.
*/
VS_OUTPUT VSCombine(VS_INPUT_COMBINE In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	Out.Pos = In.Pos;

	float2 tex = (In.Pos * 0.5f) + 0.5f;
	tex.y = 1.0f - tex.y;
	Out.Tex = tex + float2(0.5 / g_SceneTexSize.x, 0.5 / g_SceneTexSize.y);

	return Out;
}

/**
* ピクセルシェーダ.
*/
float4 PSCombine(VS_OUTPUT In) : COLOR
{
	float4 depth = tex2D(g_DepthSampler, In.Tex + float2(0.5f / g_SceneTexSize.x,0.0f));
	depth = min(depth, tex2D(g_DepthSampler, In.Tex + float2(-0.5f / g_SceneTexSize.x,0.0f)));
	depth = min(depth, tex2D(g_DepthSampler, In.Tex + float2(0.0f,-0.5f / g_SceneTexSize.y)));
	depth = min(depth, tex2D(g_DepthSampler, In.Tex + float2(0.0f,0.5f / g_SceneTexSize.y)));

	//手前ボケ
	float alpha = depth - g_DofParam.z;
	float forwardRate = max(0.0f, -g_DofParam.x - alpha);
	float backRate = max(0.0f, alpha - g_DofParam.y);

	alpha = max(forwardRate, backRate);
	alpha = alpha / ((g_DofParam.z - g_DofParam.x));
	
	float4 color = 0;

	if (forwardRate < backRate)
	{
		//奥ボケ
		alpha *= g_DofParam.x / (g_DofParam.y);
		alpha = min(1.0f, alpha);

		float4 blur = tex2D(g_BlurBackSampler, In.Tex);

		color = float4(blur.xyz, alpha);
	}
	else
	{
		//手前ボケ
		alpha = min(1.0f, alpha * 2.0f);

		float4 blur = tex2D(g_BlurForwardSampler, In.Tex);

		color = float4(blur.xyz, alpha);
	}

	return color;
}

/** テクニック. */
technique CombineBackForwardBoke
{
	pass p0 
	{
		VertexShader = compile vs_3_0 VSCombine();
		PixelShader = compile ps_3_0 PSCombine();
	}
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

float2 g_TexelOffset;

texture g_Tex;
sampler g_TextureSampler =
sampler_state
{
	Texture = <g_Tex>;
	MipFilter = NONE;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out;
	Out.Pos = In.Pos;
	Out.Tex = In.Tex + g_TexelOffset;
	return Out;
}
float4 PSMain(VS_OUTPUT In) : COLOR0
{
	return tex2D(g_TextureSampler, In.Tex);
}

technique TransformedPrim
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}