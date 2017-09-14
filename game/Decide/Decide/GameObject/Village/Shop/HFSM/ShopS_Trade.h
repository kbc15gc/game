#pragma once
#include "ShopState.h"
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
class ImageObject;
class TextObject;

//ショップの購入画面のクラス。
class ShopS_Trade :public IShopState
{
public:
	//コンストラクタ。
	//[in] ショップのポインタ。
	ShopS_Trade(Shop* shop);

	//デストラクタ。
	~ShopS_Trade();

	//毎フレーム呼ばれる更新処理。
	void Update();

	//状態に切り替えたとき呼ばれる処理。
	void EnterState();

	//現在の状態から抜ける時に呼ばれる処理。
	void ExitState();

	//子に潜る際に呼び出される処理。
	void DiveState();
private:
	void _CreateMenu();
	void _CloseMenu();
	//選択している添え字を設定。
	void SetIndex(int idx);
	//
	void SetMinIndex(int min);
	//アイテムリストを移動させる。
	void ScrollDisplayItem();
	//
	void UpdateList();
	//テキスト更新。
	void UpdateText();
	
	//売買の関数。
	function<void()> TradeFunc;

	//決定処理。
	void Decision();
	//アイテムの購入処理。
	void BuyItem();
	//アイテムの販売処理。
	void SellItem();
private:
	//現在選択している項目
	int _Select = 0;
	//表示する範囲。
	int _MinIdx = 0;
	//ショップに表示する数。
	const int DISPLAY_ITEM_NUM = 7;
	//売買する個数。
	int _SelectNum = 1;
	//ショップのステート保持。
	Shop::ShopStateE _SaveState;

	//表示するアイテムの一覧
	vector<HoldItemBase*> _DisplayList;
	//選択しているアイテム。
	Item::BaseInfo* _SelectItem;

	//カーソルの画像。
	ImageObject* _Cursor;
	//購入ウィンドウの画像。
	ImageObject* _BuyWindow;
	//アイテム一覧を視覚化したテキストのリスト。
	vector<TextObject*> _MenuTexts,_MoneyTexts;
	//メニューの一項目の縦幅。
	float _MenuListHeight;
};