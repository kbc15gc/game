#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\Village\ItemManager.h"

class ImageObject;
class TextObject;
class IShopState;

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
	struct ShopName
	{
		char name[256];
	};
}

//using句はc++11からtypedefのように型名を変えることもできる。
using ShopSPtr = shared_ptr<IShopState>;

//お店。
class Shop:public GameObject
{
public:
	//ショップのステート
	enum class ShopStateE
	{
		None = -1,
		Close,		//閉じている。
		Select,		//選択画面。
		Buy,		//購入画面。
		Sell,		//販売画面。
		Confirmation,//確認画面。
	};
public:
	Shop(const char* name);
	~Shop();

	void Awake()override;
	void Update()override;

	//ショップメニューを開く。
	void OpenShop(const unsigned int& shopID);
private:
	//
	void Close();

	//ステートをリストに追加。
	void SetState();
	//お店の情報読み込み。
	void _LoadShopData(const unsigned int& shopID);

	//ステート変更。
	void _ChangeState(const ShopStateE& state);

	//説明テキストに文字を設定する。
	void SetDescriptionText(string text);
private:
	//ふれんず。
	friend class ShopS_Close;
	friend class ShopS_Select;
	friend class ShopS_Buy;
	friend class ShopS_Confirmation;

	//ショップの名前
	vector<unique_ptr<ShopName>> _ShopNameList;
	//アイテムのリスト。
	vector<ItemBase::Item::ItemInfo*> _ItemList;
	//ショップのステート
	ShopStateE _State;
	//ショップのステートリスト。
	vector<ShopSPtr> _StateList;

	//説明のウィンドウの画像。
	ImageObject* _DescriptionWindow;
	//説明テキスト。
	TextObject* _DescriptionText;
};