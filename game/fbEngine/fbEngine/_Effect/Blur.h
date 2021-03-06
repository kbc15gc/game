/**
* ブラークラスの定義.
*/
#pragma once

/**
* ブラークラス.
*/
class Blur : Noncopyable
{
public:

	/**
	* コンストラクタ.
	*/
	Blur()
	{
	}

	/**
	* デストラクタ.
	*/
	~Blur()
	{
	}

	/**
	* 作成.
	*
	* @param w		横幅.
	* @param h		縦幅.
	* @param tex	テクスチャー.	.
	*/
	void Create(int w, int h, TEXTURE* tex);
	void Create(int w,int h, _D3DFORMAT fmt);

	/**
	* 描画.
	*/
	void Render();

	/**
	* テクスチャを取得.
	*/
	TEXTURE* GetTexture()
	{
		return _BlurRT[1].texture;
	}

	/**
	* テクスチャの設定.
	*/
	void SetTexture(TEXTURE* tex)
	{
		_SrcTexture = tex;
	}

private:

	void InitVertex();

private:

	/** 頂点を扱うクラス. */
	Vertex* _Vertex = nullptr;

	/** ブラー結果を書き込むレンダリングターゲット. */
	RenderTarget _BlurRT[2];

	/** エフェクト. */
	Effect* _Effect = nullptr;

	/** テクスチャ. */
	TEXTURE* _SrcTexture = nullptr;
	/** テクスチャのサイズ. */
	int _SrcTextureSize[2];




};