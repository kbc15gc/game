#pragma once
#include "GameObject\ItemManager\ItemManager.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Inventory\Inventory.h"

//�����A�C�e���̊��N���X�B
class HoldItemBase :public ImageObject
{
public:
	// �����F	�A�C�e���̏��\���́B
	HoldItemBase(char* name);
	~HoldItemBase();

	void Start()override;

	//�\���̂̏����擾�B
	inline Item::BaseInfo* GetInfo() {
		return _Info;
	}

	//-50����100�̒l�������_���Ŏ擾�B
	inline float GetRand_S50to100() {
		return (rand() % 151) - 50;
	}


	virtual inline void SetInfo(Item::BaseInfo* info) {
		_Info = info;
	}

	static bool SortID(HoldItemBase& left, HoldItemBase& right) 
	{
		return left.GetInfo()->ID > right.GetInfo()->ID;
	}

protected:
	Item::BaseInfo* _Info = nullptr;
};

