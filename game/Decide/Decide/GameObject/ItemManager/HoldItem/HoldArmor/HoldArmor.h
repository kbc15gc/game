#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//所持防具のクラス。
class HoldArmor :public HoldItemBase
{
public:
	HoldArmor(const char* name);
	~HoldArmor();
};