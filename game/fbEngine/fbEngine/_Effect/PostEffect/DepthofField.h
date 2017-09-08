/**
* DOF(被写界深度)クラスの定義.
*/
#pragma once

#include"_Effect\GaussianBlur.h"

class Vertex;
class Effect;

/**
* DOF(被写界深度)クラス.
*/
class DepthofField : Noncopyable
{
public:

	/**
	* コンストラクタ.
	*/
	DepthofField()
	{
	}
	
	/**
	* デストラクタ.
	*/
	~DepthofField()
	{
	}

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
	* 深度書き込み用レンダリングターゲットの取得.
	*/
	RenderTarget* GetDepthRenderTarget()
	{
		return &_DepthRT;
	}

	TEXTURE* GetBlurForward()
	{
		return _BlurForward.GetTexture();
	}
	TEXTURE* GetBlurBack()
	{
		return _BlurBack.GetTexture();
	}

	/**
	* ピントを設定.
	*/
	void SetPint(float pint)
	{
		_Pint = pint;
	}

	/**
	* 焦点距離を設定.
	*/
	void SetFocalLength(float len)
	{
		_FocalLength = len;
	}

	/**
	* F値を設定.
	*/
	void SetFParam(float f)
	{
		_F = f;
	}

	/**
	* 有効フラグを設定.
	*/
	void SetEnable(bool value)
	{
		_isEnable = value;
	}

private:

	/** 有効フラグ. */
	bool _isEnable = true;

	/** 深度書き込み用レンダリングターゲット. */
	RenderTarget _DepthRT;

	Vertex* _Vertex = nullptr;

	/** 前ボケ用ブラー. */
	GaussianBlur _BlurForward;
	/** 奥ボケ用ブラー. */
	GaussianBlur _BlurBack;

	/** ボケ合成用のレンダリングターゲット. */
	RenderTarget _CombineRenderTarget;

	/** エフェクト. */
	Effect* _Effect = nullptr;

	/** 焦点距離. */
	float _FocalLength = 26.0f;
	/** F値. */
	float _F = 5.6f;
	/** ピント. */
	float _Pint = 3.0f;

};