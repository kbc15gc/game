/**
* アイテム表示画面クラスの定義.
*/
#pragma once

/**
* アイテム表示画面クラス.
*/
class ItemWindow : public GameObject
{
public:

	/**
	* コンストラクタ.
	*/
	ItemWindow(char* name): 
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~ItemWindow()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

	/**
	* 更新.
	*/
	void Update()override;

};