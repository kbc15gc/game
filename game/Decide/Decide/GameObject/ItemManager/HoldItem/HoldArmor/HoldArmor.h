#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"

//�����h��̃N���X�B
class HoldArmor :public HoldItemBase
{
public:
	HoldArmor(Item::BaseInfo* info);
	~HoldArmor();
};