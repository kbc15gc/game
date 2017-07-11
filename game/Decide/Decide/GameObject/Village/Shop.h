#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Village\ItemManager.h"

class ImageObject;
class TextObject;

namespace
{
	enum ItemType :int
	{
		ITEM,	//回復アイテムなどの消耗品
		WEAPON,	//武器
		ARMOR,	//防具
	};

	//商品情報
	struct Product:Noncopyable
	{
	public:
		int ItemID;		//ID
		ItemType Type;	//なんのアイテムか？
	};

	const Support::DATARECORD ProductData[] =
	{
		{ "ItemID",Support::DataTypeE::INT , offsetof(struct Product,ItemID),sizeof(int) },
		{ "Type",Support::DataTypeE::INT, offsetof(struct Product,Type),sizeof(int) },
	};
}

namespace
{
	//仮
	struct testchar
	{
		char name[256];
	};
	
}

//お店。
class Shop:public GameObject
{
	//ショップのステート
	enum ShopStateE
	{
		CLOSE,		//閉じている
		SELECT,		//選択画面(買うか売るか？)
		BUY,		//購入画面。
		SELL,		//販売画面。ないかもしれない。
	};
public:
	Shop(const char* name);
	~Shop();

	void Update()override;

	//ショップメニューを開く。
	void OpenShop(const unsigned int& shopID);
private:
	//お店の情報読み込み。
	void _LoadShopData(const unsigned int& shopID);

	//メニューを閉じる。
	void _CloseMenu();

	//ステート変更。
	void _ChangeState(const ShopStateE& state);
	//入るときの処理。
	void _Enter(const ShopStateE& state);
	//出るときの処理。
	void _Exit(const ShopStateE& state);
	
	//メニュー生成。
	void _CreateMenu();

	//選択画面の更新。
	void _SelectUpdate();
	//購入画面の更新。
	void _BuyUpdate();

	//スタティック変数の初期化。
	void _StaticInit();
private:
	//ショップの名前
	vector<unique_ptr<testchar>> _ShopNameList;
	//アイテムのリスト。
	vector<Item::ItemInfo*> _ItemList;
	//ショップのステート
	ShopStateE _State;
	//ステートによって変わるアップデート
	std::function<void()> _Update;

	//カーソルの画像
	ImageObject* _Cursor[2];
	//かう・うるの画像
	ImageObject* _SelectWindow;
	//本命のウィンドウ
	ImageObject* _MainWindow;
	//メニューのテキスト
	vector<TextObject*> _MenuList;
};