#pragma once
#include "GameObject\ItemManager\ItemManager.h"

class HoldItemBase;

namespace Hold {
	//�A�C�e���̏������(�����o���Ɠǂݍ��݂Ɏg�p)�B
	struct HoldInfo
	{
		HoldInfo();
		// �����F	�A�C�e����ʁB
		//			�A�C�e���ʂ��ԍ��B
		HoldInfo(int TypeID, int ID);
		// �����F	�R�s�[���̃|�C���^�B
		HoldInfo(HoldItemBase* info);

		int _TypeID;		//�A�C�e���̎��(����A�C�e�������킩�h�)�B
		int _ID;			//�A�C�e���̒ʂ��ԍ�(TypeID�̒��Ń��j�[�N)�B
	};

	// ����A�C�e���̏������B
	struct ConsumptionInfo :public HoldInfo
	{
		ConsumptionInfo() {};
		// �����F	�A�C�e����ʁB
		//			�A�C�e���ʂ��ԍ��B
		//			�������B
		ConsumptionInfo(int TypeID, int ID, int num);

		// �����F	�R�s�[���̃|�C���^�B
		ConsumptionInfo(HoldItemBase* info);

		int _HoldNum;		//�������B
	};

	static Support::DATARECORD ConsumptionItemData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct ConsumptionInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct ConsumptionInfo,_ID),			sizeof(int) },
		{ "HoldNum",Support::DataTypeE::INT ,		offsetof(struct ConsumptionInfo,_HoldNum),		sizeof(int) },
	};

	// �����A�C�e���̏������B
	struct HoldEquipInfo : public HoldInfo {
		HoldEquipInfo();

		// �����F	�A�C�e����ʁB
		//			�A�C�e���ʂ��ԍ��B
		//			��������Ă��邩�B
		HoldEquipInfo(int TypeID, int ID, bool isEquip);

		// �����F	�R�s�[���̃|�C���^�B
		HoldEquipInfo(HoldItemBase* info);

		int _IsEquip;		//��������Ă��邩�t���O�B(ture�Ȃ瑕������Ă���Bfalse�Ȃ瑕�����ĂȂ�)�B
	};

	// ����̏������B
	struct HoldWeaponInfo : public HoldEquipInfo {
		HoldWeaponInfo() {};

		// �����F	�A�C�e����ʁB
		//			�A�C�e���ʂ��ԍ��B
		//			�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
		//			���@�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
		//			��������Ă��邩�B
		HoldWeaponInfo(int TypeID, int ID, int AtkRnd, int MAtkRnd, int CrtRnd, bool IsEquip);
		// �����F	�R�s�[���̃|�C���^�B
		HoldWeaponInfo(HoldItemBase* info);

		int _AtkRnd;		//�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
		int _MAtkRnd;		//���@�U���̗͂�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
		int _DexRnd;		//�N���e�B�J�����̗�������(���̒l�Ń����N�t���A�P�ʂ̓p�[�Z���g)�B
	};

	static Support::DATARECORD HoldWeaponData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldWeaponInfo,_ID),			sizeof(int) },
		{ "AtkRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_AtkRnd),		sizeof(int) },
		{ "MagicRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_MAtkRnd),		sizeof(int) },
		{ "DexRnd",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_DexRnd),		sizeof(int) },
		{ "IsEquip",Support::DataTypeE::INT ,		offsetof(struct HoldWeaponInfo,_IsEquip),		sizeof(int) },
	};

	// �h��̏������B
	struct HoldArmorInfo : public HoldEquipInfo {
		HoldArmorInfo() {};
		// �����F	�A�C�e����ʁB
		//			�A�C�e���ʂ��ԍ��B
		//			�������B
		//			�h��͂̃����N�����B
		//			���@�h��͂̃����N�����B
		//			��������Ă��邩�B
		HoldArmorInfo(int TypeID, int ID, int Def, int MDef, bool IsEquip);
		// �����F	�R�s�[���̃|�C���^�B
		HoldArmorInfo(HoldItemBase* info);

		int _DefRnd;	//�h��͂̃����N�����B
		int _MDefRnd;	//���@�h��͂̃����N�����B
	};

	static Support::DATARECORD HoldArmorData[] = {
		{ "TypeID",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_TypeID),			sizeof(int) },
		{ "ID",Support::DataTypeE::INT ,			offsetof(struct HoldArmorInfo,_ID),			sizeof(int) },
		{ "DefRnd",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_DefRnd),		sizeof(int) },
		{ "MDefRnd",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_MDefRnd),		sizeof(int) },
		{ "IsEquip",Support::DataTypeE::INT ,		offsetof(struct HoldArmorInfo,_IsEquip),		sizeof(int) },
	};
};


class HoldItemFactory {
public:
	// �����i�쐬�֐��B
	// �����F	�A�C�e���̏��|�C���^�B
	//			�����_���p�����[�^�𐶐����邩�B
	static HoldItemBase* CreateItem(Item::BaseInfo* info, bool isRandParam);

	// �����i�쐬�֐��B
	// �����F	�A�C�e���ۑ����|�C���^�B
	static HoldItemBase* CreateItem(Hold::HoldInfo* info);

	// �����i�쐬�֐��B
	// �����F	�A�C�e���̎�ށB
	//			�A�C�e���̔ԍ��B
	//			�����_���p�����[�^�𐶐����邩�B
	static HoldItemBase* CreateItem(Item::ItemCodeE code, int id, bool isRandParam);
};