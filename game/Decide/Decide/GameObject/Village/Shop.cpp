#include "stdafx.h"
#include "Shop.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"

Shop::Shop(const char * name):
	GameObject(name)
{
	//スタティック変数の初期化
	_StaticInit();
	//切り替え
	_ChangeState(ShopStateE::CLOSE);
}

Shop::~Shop()
{
}

void Shop::Update()
{
	if (_Update)
		_Update();
}

void Shop::OpenShop(const unsigned int & shopID)
{
	//店の商品読み込み
	_LoadShopData(shopID);
	//セレクトメニューを開く
	_ChangeState(ShopStateE::SELECT);
}

void Shop::_LoadShopData(const unsigned int& shopID)
{
	char path[256];
	sprintf(path, "Asset/Data/ShopData/%s.csv", _ShopNameList.at(shopID));
	//品揃えを読み込み
	Support::LoadCSVData<Product>(path, ProductData, ARRAY_SIZE(ProductData), _ProductList);

	//アイテムの情報を取得
	for each (Product* pro in _ProductList)
	{
		Item::ItemInfo* item = INSTANCE(ItemManager)->GetItem((unsigned int&)pro->ItemID, (unsigned int)pro->Type);

		//nullチェック
		if (item)
			_ItemList.push_back(item);
	}
}

void Shop::_ChangeState(const ShopStateE & state)
{
	_State = state;
	switch (_State)
	{
		//メニューを閉じる
	case Shop::ShopStateE::CLOSE:
		_Update = nullptr;
		_SelectWindow->SetActive(false, true);
		_MainWindow->SetActive(false, true);
		break;
		//セレクトメニュー
	case Shop::ShopStateE::SELECT:
		_Update = std::bind(&Shop::_SelectUpdate, this);
		_SelectWindow->SetActive(true, true);
		break;
	case Shop::ShopStateE::BUY:
		_Update = std::bind(&Shop::_BuyUpdate, this);
		_MainWindow->SetActive(true, true);
		break;
	case Shop::ShopStateE::SELL:
		_Update = nullptr;
		break;
	default:
		break;
	}
}

void Shop::_SelectUpdate()
{
	//現在選択している項目
	static int select = 0;
	if(KeyBoardInput->isPush(DIK_UP))
	{
		select = select > 0 ? select - 1 : 1;
	}
	else if (KeyBoardInput->isPush(DIK_DOWN))
	{
		select = (select + 1) % 2;
	}
	//カーソルをずらす。
	_Cursor[0]->transform->SetLocalPosition(Vector3(-60, 40 + -80 * select, 0));
	
	//決定(仮)
	if(KeyBoardInput->isPush(DIK_P))
	{
		if(select)
		{
			_ChangeState(Shop::ShopStateE::BUY);
		}else
		{
			_ChangeState(Shop::ShopStateE::SELL);
		}
	}
}

void Shop::_BuyUpdate()
{
	if (KeyBoardInput->isPush(DIK_P))
	{
		_ChangeState(Shop::ShopStateE::CLOSE);
	}
}

void Shop::_StaticInit()
{
	//背景
	if (_SelectWindow == nullptr)
	{
		_SelectWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopBuySell", 8);
		_SelectWindow->SetTexture(LOADTEXTURE("ShopSelect.png"));
		_SelectWindow->transform->SetPosition(Vector3(800, 200, 0));
	}
	//背景
	if (_MainWindow == nullptr)
	{
		_MainWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopMainWindouw", 8);
		_MainWindow->SetTexture(LOADTEXTURE("window.png"));
		_MainWindow->transform->SetPosition(Vector3(600, 300, 0));
	}
	//カーソル
	if (_Cursor[0] == nullptr)
	{
		_Cursor[0] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopCursor", 8);
		_Cursor[0]->SetTexture(LOADTEXTURE("ShopCursor.png"));
		_Cursor[0]->transform->SetParent(_SelectWindow->transform);
		_Cursor[0]->transform->SetLocalPosition(Vector3(-10, 10, 0));
	}
	//カーソル
	if (_Cursor[1] == nullptr)
	{
		_Cursor[1] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopMainCursor", 8);
		_Cursor[1]->SetTexture(LOADTEXTURE("ShopCursor.png"));
		_Cursor[1]->transform->SetParent(_MainWindow->transform);
		_Cursor[1]->transform->SetLocalPosition(Vector3(-10, 10, 0));
	}
	//ウィンドウを非アクティブに
	_SelectWindow->SetActive(false, true);
	_MainWindow->SetActive(false, true);
	//ショップ読み込み
	if (_ShopNameList.size() <= 0)
	{
		Support::DATARECORD shopdata[] =
		{
			"ShopName",Support::DataTypeE::STRING,0,sizeof(char) * 256
		};
		Support::LoadCSVData<testchar>("Asset/Data/ShopData/ShopName.csv", shopdata, ARRAY_SIZE(shopdata), _ShopNameList);
	}
}