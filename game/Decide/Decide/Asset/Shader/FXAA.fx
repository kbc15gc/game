//�A���`�G�C���A�X

//���͒��_
struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex0 : TEXCOORD0;
};

//�o�͒��_
struct VS_OUTPUT
{
	float4 Pos : POSITION;
	float2 Tex0 : TEXCOORD0;
};

float2 g_TexSize; //�e�N�X�`���T�C�Y

texture g_Texture;
sampler g_TextureSampler =
sampler_state
{
	Texture = <g_Texture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

//FXAA�֐�
float4 FxaaPixelShader(
	float2 TexUV, //uv���W
	float fxaaConsoleEdgeSharpness, //�G�b�W�̐؂ꖡ
	float fxaaConsoleEdgeThreshold, //�G�b�W�̏I�[
	float fxaaConsoleEdgeThresholdMin) //�G�b�W�̐�[
{
	//�Ȃ񂩂킩���Bfloat2�ł悭�ˁH
	float4 rcpFrame = float4(0.0f, 0.0f, 1.0f / g_TexSize.x, 1.0f / g_TexSize.y);

	//�ߖT4�e�N�Z�����t�F�b�`�B
	float4 NeighborTex = float4(
		TexUV.x + 0.5f / g_TexSize.x,	//�E
		TexUV.y + 0.5f / g_TexSize.y,	//��
		TexUV.x - 0.5f / g_TexSize.x,	//��
		TexUV.y - 0.5f / g_TexSize.y);	//��

	//�E���̋P�x
	float LuminanceDownRight = tex2D(g_TextureSampler, NeighborTex.xy).y;
	//�E��̋P�x
	float LuminanceUpRight = tex2D(g_TextureSampler, NeighborTex.xw).y;
	//�����̋P�x
	float LuminanceDownLeft = tex2D(g_TextureSampler, NeighborTex.zy).y;
	//����̋P�x
	float LuminanceUpLeft = tex2D(g_TextureSampler, NeighborTex.zw).y;

	//�����̐F���擾
	float4 MidColor = tex2D(g_TextureSampler, TexUV.xy);
	//�����̋P�x
	float LuminanceMid = MidColor.y; //LuminanceM

	LuminanceDownLeft += 1.0 / 384.0; //�Ȃ��ɂ��ꂥ

	 //�E���ƉE����r���đ傫������ێ�
	float LuminanceMaxDownRightUpRight = max(LuminanceDownRight, LuminanceUpRight);
	//�E���ƉE����r���ď���������ێ�
	float LuminanceMinDownRightUpRight = min(LuminanceDownRight, LuminanceUpRight);
	//�����ƍ�����r���đ傫������ێ�
	float LuminanceMaxDownLeftUpLeft = max(LuminanceDownLeft, LuminanceUpLeft);
	//�����ƍ�����r���ď���������ێ�
	float LuminanceMinDownLeftUpLeft = min(LuminanceDownLeft, LuminanceUpLeft);

	//��ԑ傫���l
	float LuminanceMax = max(LuminanceMaxDownLeftUpLeft, LuminanceMaxDownRightUpRight);
	//��ԏ������l
	float LuminanceMin = min(LuminanceMinDownLeftUpLeft, LuminanceMinDownRightUpRight);


	float LuminanceMaxScaled = LuminanceMax * fxaaConsoleEdgeThreshold;
	float LuminanceMaxScaledClamped = max(fxaaConsoleEdgeThresholdMin, LuminanceMaxScaled);

	float LuminanceMinMid = min(LuminanceMin, LuminanceMid);
	float LuminanceMaxMid = max(LuminanceMax, LuminanceMid);
	float LuminanceMaxSubMinMid = LuminanceMaxMid - LuminanceMinMid;

	if (LuminanceMaxSubMinMid < LuminanceMaxScaledClamped) {
		//�P�x�̍����������l�ȉ��������̂ŁA���̃s�N�Z���̓A���`�������Ȃ��B
		return MidColor;
	}

	//�E����������̒l
	float UpRightMinusDownLeft = LuminanceUpRight - LuminanceDownLeft;
	//��������E���̒l
	float UpLeftMinusDownRight = LuminanceUpLeft - LuminanceDownRight;

	//�P�x�̍��𗘗p���āA�M�U�M�U���������Ă���\���̍����e�N�Z�����t�F�b�`����B
	float2 dir;
	dir.x = UpRightMinusDownLeft + UpLeftMinusDownRight;
	dir.y = UpRightMinusDownLeft - UpLeftMinusDownRight;

	/*--------------------------------------------------------------------------*/
	float2 dir1 = normalize(dir.xy);

	float4 rgbyN1 = tex2D(g_TextureSampler, TexUV.xy - dir1 * rcpFrame.zw);
	float4 rgbyP1 = tex2D(g_TextureSampler, TexUV.xy + dir1 * rcpFrame.zw);
	/*--------------------------------------------------------------------------*/
	float dirAbsMinTimesC = min(abs(dir1.x), abs(dir1.y)) * fxaaConsoleEdgeSharpness;
	float2 dir2 = clamp(dir1.xy / dirAbsMinTimesC, -2.0, 2.0);
	/*--------------------------------------------------------------------------*/
	float4 rgbyN2 = tex2D(g_TextureSampler, TexUV.xy - dir2 * rcpFrame.zw);
	float4 rgbyP2 = tex2D(g_TextureSampler, TexUV.xy + dir2 * rcpFrame.zw);

	/*--------------------------------------------------------------------------*/
	//�u�����h�u�����h�B
	float4 rgbyA = rgbyN1 + rgbyP1;
	float4 rgbyB = ((rgbyN2 + rgbyP2) * 0.25) + (rgbyA * 0.25);
	/*--------------------------------------------------------------------------*/
	int twoTap = (rgbyB.y < LuminanceMin) || (rgbyB.y > LuminanceMax);

	if (twoTap) {
		//�܂��P�x�̍����傫���̂ŁA�ēx�u�����h�B
		rgbyB.xyz = rgbyA.xyz * 0.5;
	}
	return rgbyB;

}

//���_�V�F�[�_
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out;
	Out.Pos = In.Pos;
	Out.Tex0 = In.Tex0 + float2(0.5f / g_TexSize.x, 0.5f / g_TexSize.y);
	return Out;
}

//�s�N�Z���V�F�[�_
float4 PSMain(VS_OUTPUT In) : COLOR0
{
	return FxaaPixelShader(
		In.Tex0,
		1.0f,
		0.4f,
		0.0833f);
}

//�e�N�j�b�N
technique FXAA
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}
