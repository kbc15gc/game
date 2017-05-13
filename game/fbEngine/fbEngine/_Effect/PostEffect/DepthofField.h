/**
* DOF(被写界深度)クラスの定義.
*/
#pragma once

#include"_Effect\Blur.h"

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
	* 終点距離を設定.
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

private:

	/** 有効フラグ. */
	bool _isEnable = true;

	/** 深度書き込み用レンダリングターゲット. */
	RenderTarget _DepthRT;

	Vertex* _Vertex = nullptr;

	/** 前ボケ用ブラー. */
	Blur _BlurForward;
	/** 奥ボケ用ブラー. */
	Blur _BlurBack;

	/** エフェクト. */
	Effect* _Effect = nullptr;

	/** 焦点距離. */
	float _FocalLength = 0.0f;
	/** F値. */
	float _F = 0.0f;
	/** ピント. */
	float _Pint = 0.0f;

};