/**
* アイテム2D表示クラスの定義.
*/
#pragma once

#include"GameObject\ItemManager\HoldItem\HoldItemBase.h"
#include"fbEngine\_Object\_GameObject\TextObject.h"

/**
* アイテム2D描画クラス.
*/
class Item2D : public GameObject
{
public:

	/**
	* コンストラクタ.
	*/
	Item2D(char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~Item2D()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

	/**
	* アイテムデータの設定.
	*/
	void SetItemData(HoldItemBase* item);

	HoldItemBase* GetItemData()
	{
		return _ItemData;
	}

private:

	HoldItemBase* _ItemData = nullptr;

	/** 背景. */
	ImageObject* _BackWindow = nullptr;

	/** アイコン. */
	ImageObject* _Icon = nullptr;

	/** アイテム名表示. */
	TextObject* _ItemNameText = nullptr;

	/** アイテム数. */
	TextObject* _ItemCountText = nullptr;

};