#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//�������Ă��镐��̃N���X�B
class HoldWeapon :public HoldItemBase
{
public:
	HoldWeapon(const char* name);
	~HoldWeapon();
};