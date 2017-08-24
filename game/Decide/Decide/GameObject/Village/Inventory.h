#pragma once

//�C���x���g�����ɂ���A�C�e���B
namespace InventoryItem
{
	//�A�C�e���̏����܂Ƃ߂��\���́B
	struct ItemInfo :Noncopyable
	{
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
	struct ArmorInfo :Noncopyable
	{
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
	struct WeaponInfo :Noncopyable
	{
		int ID;					//����ID�B
		char Name[256];			//���햼�B
		char Description[256];	//����̐��B
		int Value;				//�l�i�B
		int ATK;				//����𑕔��������ɏオ��U���́B
		int HoldNum;			//�������B
	};
}

//�C���x���g���̘g���B
const int INVENTORYLISTNUM = 5;

#include"GameObject\Village\ItemManager.h"

//�C���x���g���N���X�B
class Inventory
{
private:
	Inventory();
public:

	//�C���x���g���̎�ށB
	enum class InventoryKodeE
	{
		ItemList = 0,
		ArmorList,
		WeaponList,
	};

	////�ǉ�����A�C�e���̎�ށB
	//enum class AddItemKodeE
	//{
	//	Item = 0,
	//	Armor,
	//	Weapon,
	//};

	static Inventory* Instance()
	{
		if (_InventoryInstance == nullptr)
		{
			_InventoryInstance = new Inventory();
			_InventoryInstance->ListInitalize();
		}
		return _InventoryInstance;
	}

	void ListInitalize();

	/*void AddPlayerInventoryList(AddItemKodeE kode,
		Item::ItemInfo *item = nullptr,
		Armor::ArmorInfo *armor = nullptr,
		Weapon::WeaponInfo *weapon = nullptr);*/

	//�v���C���[�̃A�C�e���̃C���x���g���ɒǉ��B
	void AddPlayerInventoryItem(Item::ItemInfo *item = nullptr);

	//�v���C���[�̖h��̃C���x���g���ɒǉ��B
	void AddPlayerInventoryIArmor(Armor::ArmorInfo *armor = nullptr);

	//�v���C���[�̕���̃C���x���g���ɒǉ��B
	void AddPlayerInventoryWeapon(Weapon::WeaponInfo *weapon = nullptr);

private:
	//�v���C���[�̃A�C�e���̃C���x���g���B
	InventoryItem::ItemInfo	_PlayerItemList[INVENTORYLISTNUM];

	//�v���C���[�̖h��̃C���x���g���B
	InventoryArmor::ArmorInfo	_PlayerArmorList[INVENTORYLISTNUM];

	//�v���C���[�̕���̃C���x���g���B
	InventoryWeapon::WeaponInfo	_PlayerWeapon[INVENTORYLISTNUM];

	//�v���C���[�����ݎ����Ă���A�C�e���A�h��A����̂��ꂼ��̐��B
	int ItemCounter, ArmorCounter, WeaponCounter = 0;

	static Inventory* _InventoryInstance;
};