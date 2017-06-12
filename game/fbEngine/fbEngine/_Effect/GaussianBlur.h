/**
* ガウシアンブラークラスの定義.
*/
#pragma once

/**
* ガウシアンブラークラス.
*/
class GaussianBlur : Noncopyable
{
public:

	/** ウエイトの種類. */
	enum WeightType
	{
		Weight_2,	//!< ウエイト2本
		Weight_4,	//!< ウエイト4本
		Weight_6,	//!< ウエイト6本
		Weight_8,	//!< ウエイト8本
		Weight_Num	//!< 種類数.
	};

public:

	/**
	* コンストラクタ.
	*/
	GaussianBlur()
	{
	}

	/**
	* デストラクタ.
	*/
	~GaussianBlur()
	{
	}

	/**
	* 初期化.
	*/
	void Create(int w, int h, const TEXTURE& srcTexture);
	void Create(int w, int h, _D3DFORMAT fmt);

	/**
	* ソーステクスチャを設定.
	*/
	void SetSrcTexture(const TEXTURE& texture)
	{
		_SrcTexture = &texture;
	}

	/**
	* 描画.
	*/
	void Render();

	TEXTURE* GetTexture() const
	{
		return _BlurRT[1].texture;
	}
	const RenderTarget& GetRenderTarget() const
	{
		return _BlurRT[1];
	}

	/**
	* ブラーの強度を設定.
	*/
	void SetBlurPower(float blurPower)
	{
		_BlurPower = blurPower;
	}
	
	/**
	* 使用するウェイトの数を設定.
	*/
	void SetUseWeights(WeightType w)
	{
		_WeightType = w;
	}

private:

	/**
	* 重みを計算.
	*/
	void UpdateWeight(float dispersion);

private:

	/** 頂点を扱うクラス. */
	Vertex* _Vertex = nullptr;

	/** ブラー結果を書き込むレンダリングターゲット. */
	RenderTarget _BlurRT[2];

	/** エフェクト. */
	Effect* _Effect = nullptr;

	/** テクスチャ. */
	const TEXTURE* _SrcTexture = nullptr;
	/** テクスチャのサイズ. */
	int _SrcTextureSize[2];

	/** ウエイトの種類. */
	WeightType _WeightType;
	/** ウエイトの最大数. */
	static const int MAX_WEIGHT = 8;
	/** ウエイト配列. */
	float _Weight[MAX_WEIGHT];
	/** ブラーの強さ. */
	float _BlurPower = 0;

};