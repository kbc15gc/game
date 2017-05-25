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
public:
	Shop(const char* name);
	~Shop();

	//お店の情報読み込み。
	void LoadShopData();
private:
	//購入画面表示
	void _OpenMenu();
	//スタティック変数の初期化
	void _StaticInit();
private:
	//ショップのステート
	enum ShopStateE
	{
		CLOSE,		//閉じている
		SELECT,		//選択画面(買うか売るか？)
		BUY,		//購入画面。
		SELL,		//販売画面。ないかもしれない。
	};
	//商品の品ぞろえ。
	vector<Product*> _ProductList;
	//ショップのステート
	ShopStateE _State;

	//どうせ全部同じ場所で同じ画像だろうからstatic変数。
	//カーソルの画像
	static ImageObject* _Cursor[2];
	//かう・うるの画像
	static ImageObject* _BuySell;
	//本命のウィンドウ
	static ImageObject* _MainWindow;
private:
	
	static vector<testchar*> _ShopNameList;
};