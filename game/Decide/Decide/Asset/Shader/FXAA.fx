//アンチエイリアス

//入力頂点
struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex0 : TEXCOORD0;
};

//出力頂点
struct VS_OUTPUT
{
	float4 Pos : POSITION;
	float2 Tex0 : TEXCOORD0;
};

float2 g_TexSize; //テクスチャサイズ

texture g_Texture;
sampler g_TextureSampler =
sampler_state
{
	Texture = <g_Texture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

//FXAA関数
float4 FxaaPixelShader(
	float2 TexUV, //uv座標
	float fxaaConsoleEdgeSharpness, //エッジの切れ味
	float fxaaConsoleEdgeThreshold, //エッジの終端
	float fxaaConsoleEdgeThresholdMin) //エッジの先端
{
	//なんかわからん。float2でよくね？
	float4 rcpFrame = float4(0.0f, 0.0f, 1.0f / g_TexSize.x, 1.0f / g_TexSize.y);

	//近傍4テクセルをフェッチ。
	float4 NeighborTex = float4(
		TexUV.x + 0.5f / g_TexSize.x,	//右
		TexUV.y + 0.5f / g_TexSize.y,	//下
		TexUV.x - 0.5f / g_TexSize.x,	//左
		TexUV.y - 0.5f / g_TexSize.y);	//上

	//右下の輝度
	float LuminanceDownRight = tex2D(g_TextureSampler, NeighborTex.xy).y;
	//右上の輝度
	float LuminanceUpRight = tex2D(g_TextureSampler, NeighborTex.xw).y;
	//左下の輝度
	float LuminanceDownLeft = tex2D(g_TextureSampler, NeighborTex.zy).y;
	//左上の輝度
	float LuminanceUpLeft = tex2D(g_TextureSampler, NeighborTex.zw).y;

	//中央の色を取得
	float4 MidColor = tex2D(g_TextureSampler, TexUV.xy);
	//中央の輝度
	float LuminanceMid = MidColor.y; //LuminanceM

	LuminanceDownLeft += 1.0 / 384.0; //なぁにこれぇ

	 //右下と右上を比較して大きい方を保持
	float LuminanceMaxDownRightUpRight = max(LuminanceDownRight, LuminanceUpRight);
	//右下と右上を比較して小さい方を保持
	float LuminanceMinDownRightUpRight = min(LuminanceDownRight, LuminanceUpRight);
	//左下と左上を比較して大きい方を保持
	float LuminanceMaxDownLeftUpLeft = max(LuminanceDownLeft, LuminanceUpLeft);
	//左下と左上を比較して小さい方を保持
	float LuminanceMinDownLeftUpLeft = min(LuminanceDownLeft, LuminanceUpLeft);

	//一番大きい値
	float LuminanceMax = max(LuminanceMaxDownLeftUpLeft, LuminanceMaxDownRightUpRight);
	//一番小さい値
	float LuminanceMin = min(LuminanceMinDownLeftUpLeft, LuminanceMinDownRightUpRight);


	float LuminanceMaxScaled = LuminanceMax * fxaaConsoleEdgeThreshold;
	float LuminanceMaxScaledClamped = max(fxaaConsoleEdgeThresholdMin, LuminanceMaxScaled);

	float LuminanceMinMid = min(LuminanceMin, LuminanceMid);
	float LuminanceMaxMid = max(LuminanceMax, LuminanceMid);
	float LuminanceMaxSubMinMid = LuminanceMaxMid - LuminanceMinMid;

	if (LuminanceMaxSubMinMid < LuminanceMaxScaledClamped) {
		//輝度の差がしきい値以下だったので、このピクセルはアンチをかけない。
		return MidColor;
	}

	//右上引く左下の値
	float UpRightMinusDownLeft = LuminanceUpRight - LuminanceDownLeft;
	//左上引く右下の値
	float UpLeftMinusDownRight = LuminanceUpLeft - LuminanceDownRight;

	//輝度の差を利用して、ギザギザが発生している可能性の高いテクセルをフェッチする。
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
	//ブレンドブレンド。
	float4 rgbyA = rgbyN1 + rgbyP1;
	float4 rgbyB = ((rgbyN2 + rgbyP2) * 0.25) + (rgbyA * 0.25);
	/*--------------------------------------------------------------------------*/
	int twoTap = (rgbyB.y < LuminanceMin) || (rgbyB.y > LuminanceMax);

	if (twoTap) {
		//まだ輝度の差が大きいので、再度ブレンド。
		rgbyB.xyz = rgbyA.xyz * 0.5;
	}
	return rgbyB;

}

//頂点シェーダ
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out;
	Out.Pos = In.Pos;
	Out.Tex0 = In.Tex0 + float2(0.5f / g_TexSize.x, 0.5f / g_TexSize.y);
	return Out;
}

//ピクセルシェーダ
float4 PSMain(VS_OUTPUT In) : COLOR0
{
	return FxaaPixelShader(
		In.Tex0,
		1.0f,
		0.4f,
		0.0833f);
}

//テクニック
technique FXAA
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}
