#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//所持防具のクラス。
class HoldArmor :public HoldItemBase
{
public:
	HoldArmor(Item::BaseInfo* info);
	~HoldArmor();
};