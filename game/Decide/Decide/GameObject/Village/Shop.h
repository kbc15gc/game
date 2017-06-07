#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class ImageObject;

namespace
{
	enum ItemType:int
	{
		ITEM,	//回復アイテムなどの消耗品
		WEAPON,	//武器
		ARMOR,	//防具
	};
	//アイテムの情報
	struct ItemData
	{
	public:
		int ItemID;		//ID
		ItemType Type;	//なんのアイテムか？
		char Name[256];	//アイテム名
		char Description[256];//説明。
		int Value; //価値
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

	typedef char ShopName[256];
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

	//ショップオープン。
	void OpenShop(const unsigned int& shopID);
private:
	//購入画面表示。
	void _OpenMenu();
	//お店の情報読み込み。
	void _LoadShopData(const unsigned int& shopID);
	//スタティック変数の初期化。
	void _StaticInit();

	//ステート変更。
	void _ChangeState(const ShopStateE& state);

	//選択画面の更新。
	void _SelectUpdate();
	//購入画面の更新。
	void _BuyUpdate();
private:
	//商品の品ぞろえ。
	vector<Product*> _ProductList;
	//ショップのステート
	ShopStateE _State;
	//ステートによって変わるアップデート
	std::function<void()> _Update;

	//どうせ全部同じ場所で同じ画像だろうからstatic変数。
	//カーソルの画像
	static ImageObject* _Cursor[2];
	//かう・うるの画像
	static ImageObject* _SelectWindow;
	//本命のウィンドウ
	static ImageObject* _MainWindow;
	//ショップの名前
	static vector<testchar*> _ShopNameList;
};