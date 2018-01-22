/**
* SSAOクラスの定義.
*/
#pragma once

/**
* SSAOクラス.
*/
class SSAO : Noncopyable
{
public:

	/**
	* コンストラクタ.
	*/
	SSAO()
	{
	}

	/**
	* デストラクタ.
	*/
	~SSAO()
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
	* 有効フラグを設定.
	*/
	void SetEnable(bool value)
	{
		_isEnable = value;
	}

	/**
	* 法線書き込み用レンダリングターゲットの取得.
	*/
	RenderTarget* GetNormalRenderTarget()
	{
		return &_NormalRT;
	}

private:

	/** 有効フラグ. */
	bool _isEnable = true;

	/** 法線書き込み用レンダリングターゲット. */
	RenderTarget _NormalRT;

	/** 頂点を扱うクラス. */
	Vertex* _Vertex = nullptr;

	/** エフェクトクラス. */
	Effect* _Effect = nullptr;

};