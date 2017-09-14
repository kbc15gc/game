#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//所持している武器のクラス。
class HoldWeapon :public HoldItemBase
{
public:
	HoldWeapon(Item::BaseInfo* info);
	~HoldWeapon();
private:
	int _WeaponRank = 1;	//武器のランク(デフォルトは1)。
};