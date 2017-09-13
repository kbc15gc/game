#pragma once
#include "GameObject\ItemManager\ItemManager.h"

namespace {
	//�A�C�e�����Ƃ̏�����(�����o���Ɠǂݍ��݂Ɏg�p)�B
	struct HoldNum
	{
		HoldNum() {

		}
		HoldNum(int TypeID,int ID,int HoldNum) {
			_TypeID = TypeID;
			_ID = ID;
			_HoldNum = HoldNum;
		}
		int _TypeID;		//�A�C�e���̎��(����A�C�e�������킩�h�)�B
		int _ID;			//�A�C�e���̒ʂ��ԍ�(TypeID�̒��Ń��j�[�N)�B
		int _HoldNum;	//�������B
	};
	static Support::DATARECORD HoldItemData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldNum,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldNum,_ID),			sizeof(int) },
		{ "HoldNum",Support::DataTypeE::INT ,		offsetof(struct HoldNum,_HoldNum),		sizeof(int) },
	};
}

//�����A�C�e���̊��N���X�B
class HoldItemBase
{
public:
	// �����F	�A�C�e���̏��\���́B
	HoldItemBase(Item::BaseInfo* info);
	virtual ~HoldItemBase();

	//�\���̂ɏ���ݒ�B
	inline void SetInfo(Item::BaseInfo* info) {
		_Info = info;
	}

	//�\���̂̏����擾�B
	inline Item::BaseInfo* GetInfo() {
		return _Info;
	}

	//���������X�V(���炷�ꍇ�͈����Ƀ}�C�i�X��ݒ�)�B
	inline void UpdateHoldNum(int add = 1) {
		_HoldNum += add;
	}

	//���������擾�B
	inline int GetHoldNum() {
		return  _HoldNum;
	}

private:
	int _HoldNum = 0;
	Item::BaseInfo* _Info;
};