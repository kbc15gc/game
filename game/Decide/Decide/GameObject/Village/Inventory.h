#pragma once

namespace InventoryBase
{
	struct InventoryBasemInfo :public Noncopyable
	{

	};
	//�C���x���g�����ɂ���A�C�e���B
	namespace InventoryItem
	{
		//�A�C�e���̏����܂Ƃ߂��\���́B
		struct ItemInfo :InventoryBasemInfo
		{
			int TypeID;				//��ށB
			int ID;					//�A�C�e��ID�B
			char Name[256];			//�A�C�e�����B
			char Description[256];	//�A�C�e���̐��B
			int Value;				//�l�i�B
			int Recovery;			//�򑐂��g�������̉񕜗ʁB
			int AtkBuff;			//�򑐂��g�������̍U���͂̏㏸�ʁB
			int DefBuff;			//�򑐂��g�������̖h��͂̏㏸�ʁB
			int SpeedBuff;			//�򑐂��g�������̈ړ����x�̏㏸�ʁB
			int HoldNum;			//�������B
		};
	}

	//�C���x���g�����ɂ���h��B
	namespace InventoryArmor
	{
		//�h��̏����܂Ƃ߂��\���́B
		struct ArmorInfo :InventoryBasemInfo
		{
			int TypeID;				//��ށB
			int ID;					//�h��ID�B
			char Name[256];			//�h��B
			char Description[256];	//�h��̐��B
			int Value;				//�l�i�B
			int ATK;				//�h��𑕔��������ɏオ��U���́B
			int DEF;				//�h��𑕔��������ɏオ��h��́B
			int HoldNum;			//�������B
		};
	}

	//�C���x���g�����ɂ��镐��B�B
	namespace InventoryWeapon
	{
		//�v���C���[�������Ă��镐��̏����܂Ƃ߂��\���́B
		struct WeaponInfo :InventoryBasemInfo
		{
			int TypeID;				//��ށB
			int ID;					//����ID�B
			char Name[256];			//���햼�B
			char Description[256];	//����̐��B
			int Value;				//�l�i�B
			int ATK;				//����𑕔��������ɏオ��U���́B
			int HoldNum;			//�������B
		};
	}
}
//�C���x���g���̘g���B
const int INVENTORYLISTNUM = 5;

#include"GameObject\Village\ItemManager.h"

//�C���x���g���N���X�B
class Inventory
{
private:
	Inventory();

	//�v���C���[�̃A�C�e���̃C���x���g�����������B
	void _PlayerItemListInitialize(int i);

	//�v���C���[�̖h��̃C���x���g�����������B
	void _PlayerArmorListInitialize(int i);

	//�v���C���[�̕���̃C���x���g�����������B
	void _PlayerWeaponListInitialize(int i);

	//�A�C�e�����C���x���g���ɒǉ��B
	void _AddItem(ItemBase::Item::ItemInfo *item = nullptr);

	//�h����C���x���g���ɒǉ��B
	void _AddArmor(ItemBase::Armor::ArmorInfo *armor = nullptr);

	//������C���x���g���ɒǉ��B
	void _AddWeapon(ItemBase::Weapon::WeaponInfo *weapon = nullptr);

	//�ǉ�����A�C�e�������łɒǉ�����Ă��邩�`�F�b�N���������true��Ԃ��B
	//��1����:�A�C�e���A��2����:�h��A��3����:����B
	bool _AddCheck(ItemBase::Item::ItemInfo *item = nullptr, ItemBase::Armor::ArmorInfo *armor = nullptr, ItemBase::Weapon::WeaponInfo *weapon = nullptr);
public:

	//�C���x���g���̎�ށB
	enum class InventoryKodeE
	{
		ItemList = 0,
		ArmorList,
		WeaponList,
	};

	static Inventory* Instance()
	{
		if (_InventoryInstance == nullptr)
		{
			_InventoryInstance = new Inventory();
			_InventoryInstance->ListInitalize();
		}
		return _InventoryInstance;
	}

	//�e�C���x���g���̏������B
	void ListInitalize();


	//�󂯎�����������Ƀv���C���[�̃A�C�e���̃C���x���g���ɒǉ��B
	void AddPlayerInventoryItem(ItemBase::Item::ItemInfo *item = nullptr);

	//�󂯎�����������Ƀv���C���[�̖h��̃C���x���g���ɒǉ��B
	void AddPlayerInventoryIArmor(ItemBase::Armor::ArmorInfo *armor = nullptr);

	//�󂯎�����������Ƀv���C���[�̕���̃C���x���g���ɒǉ��B
	void AddPlayerInventoryWeapon(ItemBase::Weapon::WeaponInfo *weapon = nullptr);

	//�v���C���[�̃A�C�e���̃C���x���g�����擾�B
	InventoryBase::InventoryItem::ItemInfo* GetPlayerItemList() {
		return &_PlayerItemList[0];
	}

	//�v���C���[�̖h��̃C���x���g�����擾�B
	InventoryBase::InventoryArmor::ArmorInfo* GetPlayerAromorList()
	{
		return &_PlayerArmorList[0];
	}

	//�v���C���[�̕���̃C���x���g�����擾�B
	InventoryBase::InventoryWeapon::WeaponInfo* GetPlayerWeaponList()
	{

		return &_PlayerWeapon[0];
	}

	//�~�����C���x���g�����w�肷��Ƃ��̃C���x���g�����擾�B
	InventoryBase::InventoryBasemInfo* GetInventory(InventoryKodeE kode)
	{
		switch (kode)
		{
		case Inventory::InventoryKodeE::ItemList:
			return &_PlayerItemList[0];
			break;
		case Inventory::InventoryKodeE::ArmorList:
			return &_PlayerArmorList[0];
			break;
		case Inventory::InventoryKodeE::WeaponList:
			return &_PlayerWeapon[0];
			break;
		default:
			char error[256];
			sprintf(error, "�w�肵���C���x���g���R�[�h�R�[�h�������ł��B");
			MessageBoxA(0, error, "�C���x���g���擾���s", MB_ICONWARNING);
			break;
		}
	}

private:
	//�v���C���[�̃A�C�e���̃C���x���g���B
	InventoryBase::InventoryItem::ItemInfo	_PlayerItemList[INVENTORYLISTNUM];

	//�v���C���[�̖h��̃C���x���g���B
	InventoryBase::InventoryArmor::ArmorInfo	_PlayerArmorList[INVENTORYLISTNUM];

	//�v���C���[�̕���̃C���x���g���B
	InventoryBase::InventoryWeapon::WeaponInfo	_PlayerWeapon[INVENTORYLISTNUM];

	//�A�C�e���A�h��A����̊e�C���x���g�������݂ǂꂾ���g���Ă��邩�𐔂���ϐ��B
	int UseItemListCounter, UseArmorListCounter, UseWeaponListCounter = 0;

	static Inventory* _InventoryInstance;
};