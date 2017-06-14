/**
* 海クラスの定義.
*/
#pragma once

/**
* 海クラス.
*/
class Ocean : public GameObject
{
public:

	/**
	* コンストラクタ.
	*/
	Ocean(const char* name) :GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~Ocean()
	{
	}

	/**
	* 早めの初期化.
	*/
	void Awake()override;

	/**
	* 初期化.
	*/
	void Start()override;

	/**
	* 更新.
	*/
	void Update()override;

	/**
	* 描画.
	*/
	void Render()override;

private:

	/** 頂点を扱うクラス. */
	Vertex* _Vertex = nullptr;

	/** エフェクトクラス. */
	Effect* _Effect = nullptr;

	/** 法線マップ用テクスチャ. */
	TEXTURE* _NormalTexture[2] = { nullptr };

	/** 波. */
	float _Wave = 0;

};