#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//所持している武器のクラス。
class HoldWeapon :public HoldItemBase
{
public:
	HoldWeapon(const char* name);
	~HoldWeapon();
};