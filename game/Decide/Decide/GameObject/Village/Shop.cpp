#include "stdafx.h"
#include "Shop.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
ImageObject* Shop::_Cursor[] = { nullptr,nullptr };
ImageObject* Shop::_SelectWindow = nullptr;
ImageObject* Shop::_MainWindow = nullptr;

vector<testchar*> Shop::_ShopNameList;

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
	//メニューを開く
	_OpenMenu();
}

void Shop::_OpenMenu()
{
	_ChangeState(ShopStateE::SELECT);
	_SelectWindow->SetActive(true, true);
}

void Shop::_LoadShopData(const unsigned int& shopID)
{
	char path[256];
	sprintf(path, "Asset/Data/ShopData/%s.csv", _ShopNameList.at(shopID));
	//読み込み
	Support::LoadCSVData<Product>(path, ProductData, ARRAY_SIZE(ProductData), _ProductList);
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
	if(_ShopNameList.size() <= 0)
	{
		Support::DATARECORD shopdata[] =
		{
			"ShopName",Support::DataTypeE::STRING,0,sizeof(testchar)
		};
		Support::LoadCSVData<testchar>("Asset/Data/ShopData/ShopName.csv", shopdata, ARRAY_SIZE(shopdata), _ShopNameList);
	}
}

void Shop::_ChangeState(const ShopStateE & state)
{
	_State = state;
	switch (_State)
	{
	case Shop::ShopStateE::CLOSE:
		_Update = nullptr;
		break;
	case Shop::ShopStateE::SELECT:
		_Update = std::bind(&Shop::_SelectUpdate, this);
		//_SelectWindow->SetActive(true, true);
		break;
	case Shop::ShopStateE::BUY:
		_Update = std::bind(&Shop::_BuyUpdate, this);
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
	static int a = 0;

	_Cursor[0]->transform->SetLocalPosition(Vector3(-10, 10 + 5 * a, 0));

	a = (a + 1) % 2;
}

void Shop::_BuyUpdate()
{
}
