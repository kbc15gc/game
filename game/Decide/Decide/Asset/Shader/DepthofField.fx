/**
* 被写界深度のシェーダ.
*/


/** シーンテクスチャ. */
texture g_Scene;
sampler g_SceneSampler =
sampler_state
{
	Texture = <g_Scene>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

/** シーンテクスチャサイズ. */
float2 g_SceneTexSize;

/** 深度テクスチャ. */
texture g_Depth;
sampler g_DepthSampler =
sampler_state
{
	Texture = <g_Depth>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

/** x:手前ボケ、y:奥ボケ、z:ピント. */
float3 g_DofParam;

/** 奥ボケテクスチャ. */
texture g_BlurBack;
sampler g_BlurBackSampler =
sampler_state
{
	Texture = <g_BlurBack>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

texture g_BlurForward;	//手前ボケ
sampler g_BlurForwardSampler =
sampler_state
{
	Texture = <g_BlurForward>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


/**
* 頂点シェーダ入力. 
*/
struct VS_INPUT 
{
	float4	Pos : POSITION;
};

/**
* 頂点シェーダ出力.
*/
struct VS_OUTPUT 
{
	float4 Pos 	: POSITION;
	float2 Tex : TEXCOORD0;
};

/**
* 頂点シェーダ.
*/
VS_OUTPUT VSMain(VS_INPUT In)
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
float4 PSMain(VS_OUTPUT In) : COLOR
{
	//シーンのカラー.
	float4 sceneColor = tex2D(g_SceneSampler, In.Tex);
	//深度.
	float4 depth = tex2D(g_DepthSampler, In.Tex);
	//手前ボケ
	float t = depth.x - g_DofParam.z;
	float forwardRate = max(0.0f, -g_DofParam.x - t);
	float backRate = max(0.0f, t - g_DofParam.y);

	t = max(forwardRate, backRate);
	t = t / ((g_DofParam.z - g_DofParam.x));
	
	float4 color = 0;

	if (forwardRate < backRate)
	{
		//奥ボケ
		t *= g_DofParam.x / (g_DofParam.y);
		t = min(1.0f, t);

		//駄目じゃね？
		t = max(0.0f, t);

		float4 blur = tex2D(g_BlurBackSampler, In.Tex);

		color = lerp(sceneColor, blur, t);

	}
	else
	{
		//手前ボケ
		t = min(1.0f, t * 2.0f);

		//駄目じゃね？
		t = max(0.0f, t);

		float4 blur = tex2D(g_BlurForwardSampler, In.Tex);

		color = lerp(sceneColor, blur, t);
	}

	return color;
}

/** テクニック. */
technique Dof
{
	pass p0 
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
};