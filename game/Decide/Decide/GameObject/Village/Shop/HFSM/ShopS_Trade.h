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
	//選択している添え字を設定。
	void _SetIndex(int idx);
	//売買個数変更。
	void _UpdateTradeNum();
	//売買個数加算。
	void AddTradeNum(int max);
	//売買個数減算。
	void SubTradeNum();

	//メニュー作成。
	void _CreateMenu();
	//メニューを閉じる。
	void _CloseMenu();

	//表示する項目を更新。
	void _UpdateList();
	//
	void _SetMinIndex(int min);
	//テキスト更新。
	void _UpdateText();
	//アイテム名のテキストを更新。
	void _UpdateNameText(int idx);
	//金額のテキストの更新。
	void _UpdateMoneyText(int idx);
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
	const vector<HoldItemBase*>* _DisplayList;
	//売買するアイテムの数。
	int _TradeItemNum = 0;

	//現在選択している項目
	int _Select = 0;
	//表示する範囲。
	int _MinIdx = 0;
	//ショップに表示する数。
	const int DISPLAY_ITEM_NUM = 5;
	//売買する個数。
	vector<int> _TradeNum;
	//売買するアイテムの添え字。
	vector<int> _TradeList;
	//合計金額
	int _SumValue = 0;

	//カーソルの画像。
	ImageObject* _Cursor;
	//購入ウィンドウの画像。
	ImageObject* _TradeWindow;
	//合計金額。
	TextObject* _ValueText;
	//各項目の名称説明。
	TextObject* _TopText;
	//アイテム一覧を視覚化したテキストのリスト。
	vector<TextObject*> _NameTexts,_MoneyTexts;
	//メニューの一項目の縦幅。
	float _MenuListHeight;
	//パラメータウィンドウ。
	ImageObject* _ParmWindow;
	//パラメータ
	TextObject* _ParmText;
};