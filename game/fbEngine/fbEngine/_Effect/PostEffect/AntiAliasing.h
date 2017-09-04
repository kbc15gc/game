/**
* アンチエイリアスクラスの定義.
*/
#pragma once

class Vertex;

/**
* アンチエイリアスクラス.
*/
class AntiAliasing : Noncopyable
{
public:

	/**
	* コンストラクタ.
	*/
	AntiAliasing()
	{
	}

	/**
	* デストラクタ.
	*/
	~AntiAliasing()
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

private:

	/** 有効フラグ. */
	bool _isEnable = true;

	/** 頂点を扱うクラス. */
	Vertex* _Vertex = nullptr;

	/** エフェクトクラス. */
	Effect* _Effect = nullptr;

};