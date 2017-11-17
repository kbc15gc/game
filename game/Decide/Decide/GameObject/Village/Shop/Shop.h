#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"
#include "GameObject\ItemManager\ItemManager.h"
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

#include "GameObject\Village\EventManager.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"

class ImageObject;
class TextObject;
class IShopState;

namespace
{
	//商品情報
	struct Product:Noncopyable
	{
	public:
		int ItemID;		//ID。
		Item::ItemCodeE Code;	//アイテムを識別するコード。
	};

	const Support::DATARECORD ProductData[] =
	{
		{ "ItemID",Support::DataTypeE::INT , offsetof(struct Product,ItemID),sizeof(int) },
		{ "Code",Support::DataTypeE::INT, offsetof(struct Product,Code),sizeof(int) },
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

	void OnDisable()override
	{
		INSTANCE(EventManager)->NotifyEndEvent();
	}
private:
	//ショップを閉じる。
	void Close();

	//ステートをリストに追加。
	void SetState();
	//お店の情報読み込み。
	void _LoadShopData(const unsigned int& shopID);

	//ステート変更。
	void _ChangeState(const ShopStateE state);

	//説明テキストに文字を設定する。
	void SetDescriptionText(string text);
	//ボイスの再生。
	void PlayVoice(string wavfilepath);
	//説明の設定とボイスの再生。
	void SetDescription(string text, string filepath)
	{
		SetDescriptionText(text);
		PlayVoice(filepath);
	}
	//支払い
	void Pay(int money);
private:
	//ふれんず。ステートを書いておく。
	friend class ShopS_Close;
	friend class ShopS_Select;
	friend class ShopS_Trade;
	friend class ShopS_Confirmation;

	//ショップのステート
	ShopStateE _State;
	//ショップのステートリスト。
	vector<ShopSPtr> _StateList;

	//ショップの名前
	vector<unique_ptr<ShopName>> _ShopNameList;
	//アイテムのリスト。
	vector<HoldItemBase*> _ItemList;

	//確認後に実行する関数。
	function<void()> _ShopFunc;

	//説明のウィンドウの画像。
	ImageObject* _DescriptionWindow;
	//説明テキスト。
	TextObject* _DescriptionText;
	
	//お金のウィンドウ。
	ImageObject* _MoneyWindow;
	//お金のテキスト。
	TextObject* _MoneyText;

	//ボイス再生。
	SoundSource _Voice;
};