#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//�������Ă��镐��̃N���X�B
class HoldWeapon :public HoldItemBase
{
public:
	HoldWeapon(Item::BaseInfo* info);
	~HoldWeapon();
private:
	int _WeaponRank = 1;	//����̃����N(�f�t�H���g��1)�B
};