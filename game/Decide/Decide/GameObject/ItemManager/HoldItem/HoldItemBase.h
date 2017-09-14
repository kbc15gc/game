#pragma once
#include "GameObject\ItemManager\ItemManager.h"

namespace {
	//�A�C�e�����Ƃ̏�����(�����o���Ɠǂݍ��݂Ɏg�p)�B
	struct HoldInfo
	{
		HoldInfo() {

		}
		// �����F	�A�C�e����ʁB
		//			�A�C�e���ʂ��ԍ��B
		//			�������B
		HoldInfo(int TypeID,int ID,int HoldNum) {
			_TypeID = TypeID;
			_ID = ID;
			_HoldNum = HoldNum;
		}
		int _TypeID;		//�A�C�e���̎��(����A�C�e�������킩�h�)�B
		int _ID;			//�A�C�e���̒ʂ��ԍ�(TypeID�̒��Ń��j�[�N)�B
		int _HoldNum;		//�������B
	};

	static Support::DATARECORD HoldItemData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldInfo,_ID),			sizeof(int) },
		{ "HoldNum",Support::DataTypeE::INT ,		offsetof(struct HoldInfo,_HoldNum),		sizeof(int) },
	};

	struct HoldWeponInfo : public HoldInfo {
		// �����F	�A�C�e����ʁB
		//			�A�C�e���ʂ��ԍ��B
		//			�������B
		//			�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
		//			���@�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
		HoldWeponInfo(int TypeID, int ID, int HoldNum, int AtkRnd, int MAtkRnd) : HoldInfo(TypeID, ID, HoldNum) {
			_AtkRnd = AtkRnd;
			_MAtkRnd = MAtkRnd;
		}
		int _AtkRnd;		//�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
		int _MAtkRnd;		//���@�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	};

	static Support::DATARECORD HoldWeaponData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldWeponInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldWeponInfo,_ID),			sizeof(int) },
		{ "HoldNum",Support::DataTypeE::INT ,		offsetof(struct HoldWeponInfo,_HoldNum),		sizeof(int) },
		{ "AtkRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeponInfo,_AtkRnd),		sizeof(int) },
		{ "MagicRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeponInfo,_MAtkRnd),		sizeof(int) },
	};

	struct HoldArmorInfo : public HoldInfo {
		// �����F	�A�C�e����ʁB
		//			�A�C�e���ʂ��ԍ��B
		//			�������B
		//			�h��͂̃����N�����B
		//			���@�h��͂̃����N�����B
		HoldArmorInfo(int TypeID, int ID, int HoldNum, int Def, int MDef) : HoldInfo(TypeID, ID, HoldNum) {
			_DefRnd = Def;
			_MDefRnd = MDef;
		}
		int _DefRnd;	//�h��͂̃����N�����B
		int _MDefRnd;	//���@�h��͂̃����N�����B
	};

	static Support::DATARECORD HoldArmorData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldArmorInfo,_ID),			sizeof(int) },
		{ "HoldNum",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_HoldNum),		sizeof(int) },
		{ "DefRnd",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_DefRnd),		sizeof(int) },
		{ "MDefRnd",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_MDefRnd),		sizeof(int) },
	};
}

//�����A�C�e���̊��N���X�B
class HoldItemBase
{
public:
	// �����F	�A�C�e���̏��\���́B
	HoldItemBase(Item::BaseInfo* info);
	virtual ~HoldItemBase();

	enum Rank
	{
		S = 0,
		A,
		B,
		C,
		D,
		E,
	};

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
protected:
	Rank _Rank;
};