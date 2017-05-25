#include "stdafx.h"
#include "Shop.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
ImageObject* Shop::_Cursor[] = { nullptr,nullptr };
ImageObject* Shop::_BuySell = nullptr;
ImageObject* Shop::_MainWindow = nullptr;

vector<testchar*> Shop::_ShopNameList;

Shop::Shop(const char * name):
	GameObject(name),
	_State(ShopStateE::CLOSE)
{
	//スタティック変数の初期化
	_StaticInit();
}

Shop::~Shop()
{
}

void Shop::LoadShopData()
{
	char path[256];
	char filename[128] = { "WeaponShop1" };
	sprintf(path, "Asset/Data/ShopData/%s.csv", filename);
	//読み込み
	Support::LoadCSVData<Product>(path, ProductData, ARRAY_SIZE(ProductData), _ProductList);
}

void Shop::_StaticInit()
{
	/*if (_BuySell == nullptr)
	{
		_BuySell = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopBuySell", 8);
		_BuySell->SetTexture(LOADTEXTURE("ShopSelect.png"));
		_BuySell->transform->SetPosition(Vector3(800, 200, 0));
	}

	if (_MainWindow == nullptr)
	{
		_MainWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopMainWindouw", 8);
		_MainWindow->SetTexture(LOADTEXTURE("window.png"));
		_MainWindow->transform->SetPosition(Vector3(600, 300, 0));
	}
	if (_Cursor[0] == nullptr)
	{
		_Cursor[0] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopCursor", 8);
		_Cursor[0]->SetTexture(LOADTEXTURE("ShopCursor.png"));
		_Cursor[0]->transform->SetParent(_BuySell->transform);
	}
	if (_Cursor[1] == nullptr)
	{
		_Cursor[1] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopMainCursor", 8);
		_Cursor[1]->SetTexture(LOADTEXTURE("ShopCursor.png"));
		_Cursor[1]->transform->SetParent(_MainWindow->transform);
	}

	if(_ShopNameList.size() <= 0)
	{
		Support::DATARECORD shopdata[] =
		{
			"ShopName",Support::DataTypeE::STRING,0,sizeof(char) * 256
		};
		Support::LoadCSVData<testchar>("Asset/Data/ShopData/ShopName.csv", shopdata, ARRAY_SIZE(shopdata), _ShopNameList);
	}*/
}
