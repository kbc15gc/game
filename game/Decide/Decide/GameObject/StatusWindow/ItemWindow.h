/**
* アイテム表示画面クラスの定義.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include"fbEngine\_Object\_GameObject\TextObject.h"
#include"GameObject\ItemManager\ItemManager.h"
#include"Item2D.h"
#include"GameObject\Player\Player.h"

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

	void Start()override;

	/**
	* 自分で呼ぶ初期化.
	*/
	void Init(Item::ItemCodeE code,char* name)
	{
		_ItemCode = code;
		_WindowName->SetString(name);
	}

	/**
	* 更新.
	*/
	void Update()override;

private:

	/**
	* 入力.
	*/
	void Input();

private:

	Player* _Player = nullptr;

	/** セルサイズ. */
	static const int ItemCellSize = 10;

	/** ウィンドウ名表示. */
	TextObject* _WindowName = nullptr;

	/** アイテム描画リスト. */
	vector<Item2D*> _Item2DList;

	/** アイテムコード. */
	Item::ItemCodeE _ItemCode;

	/** 現在選択中のアイテム. */
	int _NowSelectItem = 0;
	/** セレクトカーソル. */
	ImageObject* _SelectCursor = nullptr;

};