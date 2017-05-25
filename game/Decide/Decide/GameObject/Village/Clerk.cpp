#include "stdafx.h"
#include "Clerk.h"
#include "GameObject\Village\Shop.h"

Clerk::Clerk(const char * name):
	NPC(name)
{
}

void Clerk::LoadShop(int shopid)
{
	_Shop = INSTANCE(GameObjectManager)->AddNew<Shop>("Shop", 8);
	_Shop->LoadShopData();
}
