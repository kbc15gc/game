#include "stdafx.h"
#include "Shop.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
ImageObject* Shop::_Cursor[] = { nullptr,nullptr };
ImageObject* Shop::_BuySell = nullptr;
ImageObject* Shop::_MainWindow = nullptr;

Shop::Shop(const char * name):
	GameObject(name),
	_State(ShopStateE::CLOSE)
{
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
	//ì«Ç›çûÇ›
	Support::LoadCSVData<Product>(path, ProductData, ARRAY_SIZE(ProductData), _ProductList);
}

void Shop::_StaticInit()
{
	if (_Cursor[0] == nullptr)
	{
		_Cursor[0] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopCursor", 8);
		_Cursor[0]->SetTexture(LOADTEXTURE("ShopCursor.png"));
	}

	if (_Cursor[1] == nullptr)
	{
		_Cursor[1] = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopMainCursor", 8);
		_Cursor[1]->SetTexture(LOADTEXTURE("ShopCursor.png"));
	}
	if (_BuySell == nullptr)
	{
		_BuySell = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopBuySell", 8);
	}

	if (_MainWindow == nullptr)
	{
		_MainWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("ShopMainWindouw", 8);
	}
}
