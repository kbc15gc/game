#pragma once

class Vertex;
class Effect;

/**
* ブルームのポストエフェクト.
*/
class Bloom
{
public:

	/**
	* コンストラクタ.
	*/
	Bloom(){}

	/**
	* デストラクタ.
	*/
	~Bloom(){}

	/**
	* 作成.
	*/
	void Create();
	
	/**
	* 描画.
	*/
	void Render();

	/**
	* 解放.
	*/
	void Release();

	/**
	* 有効フラグを設定.
	*/
	void SetEnable(bool value)
	{
		_IsEnable = value;
	}

	/**
	* 輝度用レンダリングターゲットを取得.
	*/
	RenderTarget* GetLuminanceRT()
	{
		if (_IsEnable)
		{
			return _LuminanceRT;
		}
		return nullptr;
	}

private:

	/**
	* 重みを計算.
	*/
	void _UpdateWeight(const float& dis);

private:

	//
	Vertex* _Vertex;
	//有効フラグ. 
	bool _IsEnable = true;

	//重みの数
	static const int NUM_WEIGHTS = 8;
	//重み
	float _Weights[NUM_WEIGHTS];

	//エフェクト
	Effect* _Effect;

	/**
	* 輝度用のレンダリングターゲット
	*/
	RenderTarget* _LuminanceRT;

	/** 輝度カラー用のレンダリングターゲット. */
	RenderTarget* _LuminanceColorRT = nullptr;

	//ぼかし合成用のRT
	RenderTarget* _CombineRT;

	//ダウンサンプリング用RTの数
	static const int NUM_DOWN_SAMPLING_RT = 10;
	//輝度をダウンサンプリングするためのRT
	RenderTarget* _DownSamplingRT[NUM_DOWN_SAMPLING_RT];
};