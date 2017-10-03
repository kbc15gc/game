/**
* ダイアログクラスの定義.
*/
#pragma once

#include"Item2D.h"

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

	/** アイテム. */
	HoldItemBase* _Item = nullptr;

	/** 背景. */
	ImageObject* _BackWindow = nullptr;

	/** カーソルオブジェクト. */
	ImageObject* _CursorObject = nullptr;

	/** コマンドリスト. */
	vector<TextObject*> _CommandList;

};