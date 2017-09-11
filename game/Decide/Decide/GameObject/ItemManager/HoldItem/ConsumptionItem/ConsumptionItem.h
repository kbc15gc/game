#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
class Player;

//����A�C�e���̃N���X�B
class ConsumptionItem :public HoldItemBase
{
public:
	ConsumptionItem(const char* name);
	~ConsumptionItem();

	//�A�C�e�����g���B
	void UseItem(Item::ItemInfo* item);
private:
	Player* Target = nullptr;	//����A�C�e�����g���ΏہB
};