/**
* ��ʊE�[�x�̃V�F�[�_.
*/


/** �V�[���e�N�X�`��. */
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

/** �V�[���e�N�X�`���T�C�Y. */
float2 g_SceneTexSize;

/** �[�x�e�N�X�`��. */
texture g_Depth;
sampler g_DepthSampler =
sampler_state
{
	Texture = <g_Depth>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

/** x:��O�{�P�Ay:���{�P�Az:�s���g. */
float3 g_DofParam;

/** ���{�P�e�N�X�`��. */
texture g_BlurBack;
sampler g_BlurBackSampler =
sampler_state
{
	Texture = <g_BlurBack>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

texture g_BlurForward;	//��O�{�P
sampler g_BlurForwardSampler =
sampler_state
{
	Texture = <g_BlurForward>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


/**
* ���_�V�F�[�_����. 
*/
struct VS_INPUT 
{
	float4	Pos : POSITION;
};

/**
* ���_�V�F�[�_�o��.
*/
struct VS_OUTPUT 
{
	float4 Pos 	: POSITION;
	float2 Tex : TEXCOORD0;
};

/**
* ���_�V�F�[�_.
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
* �s�N�Z���V�F�[�_.
*/
float4 PSMain(VS_OUTPUT In) : COLOR
{
	//�V�[���̃J���[.
	float4 sceneColor = tex2D(g_SceneSampler, In.Tex);
	//�[�x.
	float4 depth = tex2D(g_DepthSampler, In.Tex);
	//��O�{�P
	float t = depth.x - g_DofParam.z;
	float forwardRate = max(0.0f, -g_DofParam.x - t);
	float backRate = max(0.0f, t - g_DofParam.y);

	t = max(forwardRate, backRate);
	t = t / ((g_DofParam.z - g_DofParam.x));
	
	float4 color = 0;

	if (forwardRate < backRate)
	{
		//���{�P
		t *= g_DofParam.x / (g_DofParam.y);
		t = min(1.0f, t);

		//�ʖڂ���ˁH
		t = max(0.0f, t);

		float4 blur = tex2D(g_BlurBackSampler, In.Tex);

		color = lerp(sceneColor, blur, t);

	}
	else
	{
		//��O�{�P
		t = min(1.0f, t * 2.0f);

		//�ʖڂ���ˁH
		t = max(0.0f, t);

		float4 blur = tex2D(g_BlurForwardSampler, In.Tex);

		color = lerp(sceneColor, blur, t);
	}

	return color;
}

/** �e�N�j�b�N. */
technique Dof
{
	pass p0 
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
};