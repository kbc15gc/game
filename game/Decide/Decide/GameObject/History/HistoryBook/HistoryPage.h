/**
* 歴史書のページクラスの定義.
*/
#pragma once

/**
* 歴史書のページクラス.
*/
class HistoryPage : public GameObject
{
public:

	/**
	* コンストラクタ.
	*/
	HistoryPage(const char* name) : 
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~HistoryPage()
	{
	}

	/**
	* コンストラクタ後の初期化.
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

private:

	/** ページのモデル. */
	SkinModel* _Model = nullptr;

};