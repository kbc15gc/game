#pragma once
#include "GameObject\ItemManager\ItemManager.h"

//所持アイテムの基底クラス。
class HoldItemBase :public GameObject
{
public:
	HoldItemBase(const char* name);
	~HoldItemBase();
	Item::BaseInfo* _Info = nullptr;
};