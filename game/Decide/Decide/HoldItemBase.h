#pragma once
#include "GameObject\ItemManager\ItemManager.h"

//�����A�C�e���̊��N���X�B
class HoldItemBase :public GameObject
{
public:
	HoldItemBase(const char* name);
	~HoldItemBase();
	Item::BaseInfo* _Info = nullptr;
};