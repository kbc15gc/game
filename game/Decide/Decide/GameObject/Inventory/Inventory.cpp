#include "stdafx.h"
#include "Inventory.h"

Inventory* Inventory::_InventoryInstance = nullptr;

Inventory::Inventory()
{
}

//�e�C���x���g���̏������B
void Inventory::ListInitalize()
{
	//�t�@�C���l�[��
	const char* filename[] = { "Item","Armor","Weapon", };
	FOR(i, ARRAY_SIZE(filename)) {
		switch (i)
		{
			//�v���C���[�̃A�C�e���̃C���x���g���B
		case (int)ItemManager::ItemKodeE::Item:

			for (int j = 0; j < INVENTORYLISTNUM; j++)
			{
				_PlayerItemListInitialize(j);
			}
			break;
			//�v���C���[�̖h��̃C���x���g���B
		case (int)ItemManager::ItemKodeE::Armor:

			for (int j = 0; j < INVENTORYLISTNUM; j++)
			{
				_PlayerArmorListInitialize(j);
			}
			break;
			//�v���C���[�̕���̃C���x���g���B
		case (int)ItemManager::ItemKodeE::Weapon:

			for (int j = 0; j < INVENTORYLISTNUM; j++)
			{
				_PlayerWeaponListInitialize(j);
			}
			break;
		}
	}
}
//�v���C���[�̃A�C�e���̃C���x���g�����������B
void Inventory::_PlayerItemListInitialize(int i) {
	_PlayerItemList[i].TypeID = -1;
	_PlayerItemList[i].ID = -1;
	strcpy(_PlayerItemList[i].Name, "None");
	strcpy(_PlayerItemList[i].Description, "None");
	_PlayerItemList[i].Value = -1;
	_PlayerItemList[i].Recovery = -1;
	_PlayerItemList[i].AtkBuff = -1;
	_PlayerItemList[i].DefBuff = -1;
	_PlayerItemList[i].SpeedBuff = -1;
	_PlayerItemList[i].HoldNum = 0;
}

//�v���C���[�̖h��̃C���x���g�����������B
void Inventory::_PlayerArmorListInitialize(int i)
{
	_PlayerArmorList[i].TypeID = -1;
	_PlayerArmorList[i].ID = -1;
	strcpy(_PlayerArmorList[i].Name, "None");
	strcpy(_PlayerArmorList[i].Description, "None");
	_PlayerArmorList[i].Value = -1;
	_PlayerArmorList[i].ATK = -1;
	_PlayerArmorList[i].DEF = -1;
	_PlayerArmorList[i].HoldNum = 0;
}

//�v���C���[�̕���̃C���x���g�����������B
void Inventory::_PlayerWeaponListInitialize(int i)
{
	_PlayerWeaponList[i].TypeID = -1;
	_PlayerWeaponList[i].ID = -1;
	strcpy(_PlayerWeaponList[i].Name, "None");
	strcpy(_PlayerWeaponList[i].Description, "None");
	_PlayerWeaponList[i].Value = -1;
	_PlayerWeaponList[i].ATK = -1;
	_PlayerWeaponList[i].HoldNum = 0;
}

//�C���x���g���ɃA�C�e����ǉ�����B
void Inventory::AddInventory(ItemManager::ItemKodeE kode, Item::BaseInfo * item)
{
	//NULL�`�F�b�N�B
	if (NULL != item)
	{
		switch (kode)
		{
			//�A�C�e���̒ǉ��B
		case ItemManager::ItemKodeE::Item:
			//�ǉ�����A�C�e�����������𑝂₷�A�C�e�����`�F�b�N�B
			_ItemAddCheckAndPos((Item::ItemInfo*)item);	
			break;

			//�h��̒ǉ��B
		case ItemManager::ItemKodeE::Armor:
			//�ǉ�����h��������𑝂₷�A�C�e�����`�F�b�N�B
			_ArmorAddCheckAndPos((Item::ArmorInfo*)item);
			break;

			//����̒ǉ��B
		case ItemManager::ItemKodeE::Weapon:
			//�ǉ����镐�킩�������𑝂₷�A�C�e�����`�F�b�N�B
			_WeaponAddCheckAndPos((Item::WeaponInfo*)item);
			break;

			//��O�����B
		default:
			char error[256];
			sprintf(error, "�w�肵���A�C�e���R�[�h�R�[�h�������ł��B");
			MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
			break;
		}
	}
	else
	{
		char error[256];
		sprintf(error, "�ǉ�����A�C�e����NULL�ł����B");
		MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
	}
}

//�ǉ�����A�C�e�����A�C�e���̃C���x���g������T������Ώ����������𑝂₵�A�Ȃ���΋󂢂Ă���ꏊ�ɒǉ����A�ǂ���ł��Ȃ���΃G���[���o���B
void Inventory::_ItemAddCheckAndPos(Item::ItemInfo *item)
{
	//�A�C�e���̃C���x���g���̋󂫂������B
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//�󂫃`�F�b�N�B
		if (_PlayerItemList[i].TypeID == -1) {

			//�ǉ������B
			_AddItem(i, item);
			return;

		}
		//�����A�C�e���������B
		else if(_PlayerItemList[i].ID == item->ID)
		{
			//�������𑝉��B
			_PlayerItemList[i].HoldNum++;
			return;
		}
	}

	char error[256];
	sprintf(error, "�A�C�e���̃C���x���g������t�Œǉ��ł��܂���ł����B");
	MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
}

void Inventory::_ArmorAddCheckAndPos(Item::ArmorInfo *armor)
{
	//�A�C�e���̃C���x���g���̋󂫂������B
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//�󂫃`�F�b�N�B
		if (_PlayerArmorList[i].TypeID == -1) {

			//�ǉ������B
			_AddArmor(i, armor);
			return;

		}
		//�����A�C�e���������B
		else if (_PlayerArmorList[i].ID == armor->ID)
		{
			//�������𑝉��B
			_PlayerArmorList[i].HoldNum++;
			return;
		}
	}

	char error[256];
	sprintf(error, "�h��̃C���x���g������t�Œǉ��ł��܂���ł����B");
	MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
}

void Inventory::_WeaponAddCheckAndPos(Item::WeaponInfo* weapon)
{
	//�A�C�e���̃C���x���g���̋󂫂������B
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//�󂫃`�F�b�N�B
		if (_PlayerArmorList[i].TypeID == -1) {

			//�ǉ������B
			_AddWeapon(i, weapon);
			return;

		}
		//�����A�C�e���������B
		else if (_PlayerArmorList[i].ID == weapon->ID)
		{
			//�������𑝉��B
			_PlayerArmorList[i].HoldNum++;
			return;
		}
	}
	char error[256];
	sprintf(error, "�A�C�e���̃C���x���g������t�Œǉ��ł��܂���ł����B");
	MessageBoxA(0, error, "�C���x���g���ɒǉ����s", MB_ICONWARNING);
}

//�A�C�e�����C���x���g���ɒǉ��B
void Inventory::_AddItem(int AddPos, Item::ItemInfo *item)
{
	_PlayerItemList[AddPos].TypeID = item->TypeID;
	_PlayerItemList[AddPos].ID = item->ID;
	strcpy(_PlayerItemList[AddPos].Name, item->Name);
	strcpy(_PlayerItemList[AddPos].Description, item->Description);
	_PlayerItemList[AddPos].Value = item->Value;
	_PlayerItemList[AddPos].Recovery = item->Recovery;
	_PlayerItemList[AddPos].AtkBuff = item->AtkBuff;
	_PlayerItemList[AddPos].DefBuff = item->DefBuff;
	_PlayerItemList[AddPos].SpeedBuff = item->SpeedBuff;
	_PlayerItemList[AddPos].HoldNum++;
}

//�h����C���x���g���ɒǉ��B
void Inventory::_AddArmor(int AddPos, Item::ArmorInfo *armor)
{
	_PlayerArmorList[AddPos].TypeID = armor->TypeID;
	_PlayerArmorList[AddPos].ID = armor->ID;
	strcpy(_PlayerArmorList[AddPos].Name, armor->Name);
	strcpy(_PlayerArmorList[AddPos].Description, armor->Description);
	_PlayerArmorList[AddPos].Value = armor->Value;
	_PlayerArmorList[AddPos].ATK = armor->ATK;
	_PlayerArmorList[AddPos].DEF = armor->DEF;
}

//������C���x���g���ɒǉ��B
void Inventory::_AddWeapon(int AddPos, Item::WeaponInfo *weapon)
{
	_PlayerWeaponList[AddPos].TypeID = weapon->TypeID;
	_PlayerWeaponList[AddPos].ID = weapon->ID;
	strcpy(_PlayerWeaponList[AddPos].Name, weapon->Name);
	strcpy(_PlayerArmorList[AddPos].Description, weapon->Description);
	_PlayerWeaponList[AddPos].Value = weapon->Value;
	_PlayerWeaponList[AddPos].ATK = weapon->ATK;
	UseWeaponListCounter++;
}