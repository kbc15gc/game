#pragma once
#include "HoldItemBase.h"

//所持している武器のクラス。
class HoldWeapon :public HoldItemBase
{
public:
	HoldWeapon(const char* name);
	~HoldWeapon();
};