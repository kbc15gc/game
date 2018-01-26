#pragma once
#include "GameObject\ItemManager\ItemManager.h"
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "GameObject\Inventory\Inventory.h"
#include "GameObject\ItemManager\HoldItem\HoldItemFactory.h"

//�����A�C�e���̊��N���X�B
class HoldItemBase :public GameObject
{
protected:
	friend class HoldItemFactory;
	HoldItemBase(char* name);
public:
	~HoldItemBase();

	void Start()override;

	// �O������ǂݍ��񂾃f�[�^��ݒ�B
	// �����F	CSV�ǂݏ����p�̏��������i�\���̂ւ̃|�C���^�B
	// ��CSV����ǂݍ��񂾃����_���p�����[�^���⑕�������g�p����ۂ͂��̊֐��Ńp�����[�^��ݒ肷��B
	virtual void ConfigLoadData(Hold::HoldInfo* info) = 0;

	//�\���̂̏����擾�B
	inline Item::BaseInfo* GetInfo() {
		return _Info;
	}

	int GetValue()
	{
		return _Info->Value;
	}

	//-50����100�̒l�������_���Ŏ擾�B
	//�p�����[�^��␳����l�������_���ŎZ�o�B
	inline int GetRand_S50to100() {
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

