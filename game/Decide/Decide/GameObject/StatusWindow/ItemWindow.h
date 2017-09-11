/**
* アイテム表示画面クラスの定義.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include"fbEngine\_Object\_GameObject\TextObject.h"

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

private:

	/** アイテム描画リスト. */
	vector<TextObject*> _ItemTextList;

};