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
	//タブの切り替え。
	void _SwitchTab();
	//売買個数変更。
	void _UpdateTradeNum();

	//メニュー作成。
	void _CreateMenu();
	//メニューを閉じる。
	void _CloseMenu();

	//表示する項目を更新。
	void _UpdateList();
	//選択している添え字を設定。
	void _SetIndex(int idx);
	//
	void _SetMinIndex(int min);
	//テキスト更新。
	void _UpdateText();
	//アイテムリストを移動させる。
	void _ScrollDisplayItem();

	//アイテムの情報をテキストに送信。
	void _SendItemInfo(HoldItemBase* item);
	//カラーコードを算出。
	char* _CalcColorCode(int diff);
	
	//決定処理。
	void _Decision();
	//アイテムの購入処理。
	void BuyItem();
	//アイテムの販売処理。
	void SellItem();
private:
	//プレイヤーへの参照。
	Player* player;
	Player* GetPlayer()
	{
		if(player == nullptr)
		{
			player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
		}
		return player;
	}
private:
	//ショップのステート保持。
	Shop::ShopStateE _SaveState;

	//表示するアイテムの種類。
	int _DisplayType = static_cast<int>(Item::ItemCodeE::Item);
	//表示するアイテムの一覧。
	vector<HoldItemBase*> _DisplayList;
	//アイテムの数。
	int _DisplayItemNum = 0;

	//現在選択している項目
	int _Select = 0;
	//表示する範囲。
	int _MinIdx = 0;
	//ショップに表示する数。
	const int DISPLAY_ITEM_NUM = 7;
	//売買する個数。
	vector<int> _TradeNum;
	//選択しているアイテム。
	Item::BaseInfo* _SelectItem;

	//カーソルの画像。
	ImageObject* _Cursor;
	//購入ウィンドウの画像。
	ImageObject* _TradeWindow;
	//アイテム一覧を視覚化したテキストのリスト。
	vector<TextObject*> _MenuTexts,_MoneyTexts;
	//メニューの一項目の縦幅。
	float _MenuListHeight;
	//パラメータウィンドウ。
	ImageObject* _ParmWindow;
	//パラメータ
	TextObject* _ParmText;
};