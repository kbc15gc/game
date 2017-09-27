/**
* アイテム表示画面クラスの定義.
*/
#pragma once

#include"fbEngine\_Object\_GameObject\ImageObject.h"
#include"fbEngine\_Object\_GameObject\TextObject.h"
#include"GameObject\ItemManager\ItemManager.h"
#include"Item2D.h"
#include"GameObject\Player\Player.h"
#include"ParameterRender.h"
#include"HoldItem2D.h"

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
	* 自分で呼ぶ初期化.
	*/
	void Init(Item::ItemCodeE code);

	/**
	* 更新.
	*/
	void Update()override;

private:

	/**
	* 消費アイテムの初期化.
	*/
	void ItemInit();

	/**
	* 武器の初期化.
	*/
	void WeaponInit();
	
	/**
	* 防具の初期化.
	*/
	void ArmorInit();

	/**
	* 入力.
	*/
	void Input();

private:

	Player* _Player = nullptr;

	/** セルサイズ. */
	static const int ItemCellSize = 5;

	/** ウィンドウ名表示. */
	TextObject* _WindowName = nullptr;

	/** アイテム描画リスト. */
	vector<Item2D*> _Item2DList;

	/** アイテムコード. */
	Item::ItemCodeE _ItemCode;

	/** 現在選択中のアイテム. */
	int _NowSelectItem = 0;
	/** リストの初めの添え字. */
	int _StartLoadCount = 0;
	/** セレクトカーソル. */
	ImageObject* _SelectCursor = nullptr;

	/** Eアイコン. */
	ImageObject* _EIconImage = nullptr;

	/** パラメーターリスト. */
	vector<ParameterRender*> _ParameterRenderList;

	/** 装備アイテムリスト. */
	vector<HoldItem2D*> _HoldItem2DList;

};