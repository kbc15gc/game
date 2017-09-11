#pragma once
#include "GameObject\ItemManager\ItemManager.h"

//�����A�C�e���̊��N���X�B
class HoldItemBase :public GameObject
{
public:
	HoldItemBase(const char* name);
	~HoldItemBase();
	

	//�V���b�v�ŃA�C�e���𔄂������������������炷�B
	void ItemSub(int sub = 1);

	//�\���̂ɏ���ݒ�B
	inline void SetInfo(Item::BaseInfo* info) {
		_Info = info;
	}

	//�\���̂̏����擾�B
	inline Item::BaseInfo* GetInfo() {
		return _Info;
	}

	//�������𑝂₷�B
	inline void AddHoldNum(int add = 1) {
		_HoldNum = add;
	}

private:
	Item::BaseInfo* _Info = nullptr;	//�A�C�e���̍\���́B
	int _HoldNum;						//�������B
};