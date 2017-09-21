#pragma once
#include "GameObject\ItemManager\HoldItem\HoldItemBase.h"
class Player;

//����A�C�e���̃N���X�B
class ConsumptionItem :public HoldItemBase
{
public:
	ConsumptionItem(Item::BaseInfo* info);
	~ConsumptionItem();

	//�A�C�e�����g���B
	void UseItem(Item::ItemInfo* item);

	//���������X�V(���炷�ꍇ�͈����Ƀ}�C�i�X��ݒ�)�B
	inline void UpdateHoldNum(int add = 1) {
		_HoldNum += add;
	}

	//���������擾�B
	inline int GetHoldNum() {
		return  _HoldNum;
	}
private:
	Player* _UseTarget = nullptr;	//����A�C�e�����g���ΏہB

	int _HoldNum = 0;
};