/**
* ダイアログクラスの定義.
*/
#pragma once

#include"Item2D.h"
#include "GameObject\StatusWindow\Cursor.h"

/**
* ダイアログクラス.
*/
class Dialog : public GameObject
{
public:

	/**
	* コマンド.
	*/
	enum DialogCommand
	{
		None = -1,		//!< なし.
		Equip,			//!< 装備.
		Dump,			//!< 捨てる.
		UseItem,		//!< アイテム使用.
		EquipItemUp,	//!< アイテム装備↑.
		EquipItemDwon,	//!< アイテム装備↓.
		EquipItemLeft,	//!< アイテム装備←.
		EquipItemRight,	//!< アイテム装備→.
	};

public:

	/**
	* コンストラクタ.
	*/
	Dialog(char* name) :
		GameObject(name)
	{
	}

	/**
	* デストラクタ.
	*/
	~Dialog()
	{
	}

	/**
	* 初期化.
	*/
	void Awake()override;

	/**
	* 初期化.
	*/
	void Init(Item::ItemCodeE code);

	/**
	* 更新.
	*/
	DialogCommand InputUpdate();

	/**
	* 有効にする.
	*/
	void Enable(Item2D* item);

private:

	/** 背景. */
	ImageObject* _BackWindow = nullptr;

	/** カーソルオブジェクト. */
	ImageObject* _CursorObject = nullptr;

	/** コマンドリスト. */
	vector<TextObject*> _CommandList;

	int _NowSelect = 0;

	/** カーソル. */
	Cursor* _Cursor = nullptr;

	Item::ItemCodeE _ItemCode;

};