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

//�v���C���[�̃A�C�e���̃C���x���g���ɒǉ��B
void Inventory::AddPlayerInventoryItem(Item::ItemInfo *item)
{
	////�������Ă���A�C�e���̒��ɒǉ�����A�C�e���Ɠ������̂��Ȃ����`�F�b�N�B
	//for (int i = 0; i < UseItemListCounter; i++)
	//{
	//	//�����A�C�e���Ȃ̂ŏ������̍X�V�B
	//	if (_PlayerItemList[i].ID = item->ID)
	//	{
	//		_PlayerItemList[i].HoldNum++;
	//		break;
	//	}
	//}

	//if (_AddCheck(item, NULL, NULL) != false) {

		//�ǉ�����A�C�e���̏���ǉ��B
		_AddItem(item);

	//}

	
}

//�v���C���[�̖h��̃C���x���g���ɒǉ��B
void Inventory::AddPlayerInventoryIArmor(Item::ArmorInfo *armor)
{
	////�������Ă���h��̒��ɒǉ�����h��Ɠ������̂��Ȃ����`�F�b�N�B
	//for (int i = 0; i < UseArmorListCounter; i++)
	//{
	//	//�����h��Ȃ̂ŏ������̍X�V�B
	//	if (_PlayerArmorList[i].ID = armor->ID)
	//	{
	//		_PlayerArmorList[i].HoldNum++;
	//		break;
	//	}
	//}

	//if (_AddCheck(NULL, armor, NULL) != false) {

		//�ǉ�����h��̏���ǉ��B
		_AddArmor(armor);

	//}
}

//�v���C���[�̕���̃C���x���g���ɒǉ��B
void Inventory::AddPlayerInventoryWeapon(Item::WeaponInfo *weapon)
{
	////�������Ă��镐��̒��ɒǉ����镐��Ɠ������̂��Ȃ����`�F�b�N�B
	//for (int i = 0; i < UseArmorListCounter; i++)
	//{
	//	//��������Ȃ̂ŏ������̍X�V�B
	//	if (_PlayerWeapon[i].ID = weapon->ID)
	//	{
	//		_PlayerWeapon[i].HoldNum++;
	//		break;
	//	}
	//}

	//if (_AddCheck(NULL, NULL,weapon) != false) {

		//�ǉ����镐��̏���ǉ��B
		_AddWeapon(weapon);
	//}
	
}

void Inventory::AddInventory(ItemManager::ItemKodeE kode ,Item::BaseInfo * item)
{
	switch (kode)
	{
	case ItemManager::ItemKodeE::Item:
		_AddItem((Item::ItemInfo*)item);
		break;

	case ItemManager::ItemKodeE::Armor:
		_AddArmor((Item::ArmorInfo*)item);
		break;

	case ItemManager::ItemKodeE::Weapon:
		_AddWeapon((Item::WeaponInfo*)item);
		break;
	default:
		break;
	}
}

//�v���C���[�̃A�C�e���̃C���x���g�����������B
void Inventory::_PlayerItemListInitialize(int i) {
	_PlayerItemList[i].ID = -1;
	strcpy(_PlayerItemList[i].Name, "None");
	strcpy(_PlayerItemList[i].Description, "None");
	_PlayerItemList[i].Value = -1;
	_PlayerItemList[i].Recovery = -1;
	_PlayerItemList[i].AtkBuff = -1;
	_PlayerItemList[i].DefBuff = -1;
	_PlayerItemList[i].HoldNum = 0;
	_PlayerItemList[i].SpeedBuff = -1;
}

//�v���C���[�̖h��̃C���x���g�����������B
void Inventory::_PlayerArmorListInitialize(int i)
{
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
	_PlayerWeapon[i].ID = -1;
	strcpy(_PlayerWeapon[i].Name, "None");
	strcpy(_PlayerWeapon[i].Description, "None");
	_PlayerWeapon[i].Value = -1;
	_PlayerWeapon[i].ATK = -1;
	_PlayerWeapon[i].HoldNum = 0;
}

//�A�C�e�����C���x���g���ɒǉ��B
void Inventory::_AddItem(Item::ItemInfo *item)
{
	_PlayerItemList[UseItemListCounter].TypeID = item->TypeID;
	_PlayerItemList[UseItemListCounter].ID = item->ID;
	strcpy(_PlayerItemList[UseItemListCounter].Name, item->Name);
	strcpy(_PlayerItemList[UseItemListCounter].Description, item->Description);
	_PlayerItemList[UseItemListCounter].Value = item->Value;
	_PlayerItemList[UseItemListCounter].Recovery = item->Recovery;
	_PlayerItemList[UseItemListCounter].AtkBuff = item->AtkBuff;
	_PlayerItemList[UseItemListCounter].DefBuff = item->DefBuff;
	_PlayerItemList[UseItemListCounter].SpeedBuff = item->SpeedBuff;
	_PlayerItemList[UseItemListCounter].HoldNum++;
	UseItemListCounter++;

}

//�h����C���x���g���ɒǉ��B
void Inventory::_AddArmor(Item::ArmorInfo *armor)
{
	_PlayerArmorList[UseArmorListCounter].TypeID = armor->TypeID;
	_PlayerArmorList[UseArmorListCounter].ID = armor->ID;
	strcpy(_PlayerArmorList[UseArmorListCounter].Name, armor->Name);
	strcpy(_PlayerArmorList[UseArmorListCounter].Description, armor->Description);
	_PlayerArmorList[UseArmorListCounter].Value = armor->Value;
	_PlayerArmorList[UseArmorListCounter].ATK = armor->ATK;
	_PlayerArmorList[UseArmorListCounter].DEF = armor->DEF;
	UseArmorListCounter++;
}

//������C���x���g���ɒǉ��B
void Inventory::_AddWeapon(Item::WeaponInfo *weapon)
{
	_PlayerWeapon[UseWeaponListCounter].TypeID = weapon->TypeID;
	_PlayerWeapon[UseWeaponListCounter].ID = weapon->ID;
	strcpy(_PlayerWeapon[UseWeaponListCounter].Name, weapon->Name);
	strcpy(_PlayerArmorList[UseWeaponListCounter].Description, weapon->Description);
	_PlayerWeapon[UseWeaponListCounter].Value = weapon->Value;
	_PlayerWeapon[UseWeaponListCounter].ATK = weapon->ATK;
	UseWeaponListCounter++;
}

//�ǉ�����A�C�e�������łɒǉ�����Ă��邩�`�F�b�N���������true��Ԃ�
bool Inventory::_AddCheck(Item::ItemInfo *item , Item::ArmorInfo *armor, Item::WeaponInfo *weapon)
{
	//�A�C�e���̃`�F�b�N�B
	if (NULL != item) 
	{
		//�������Ă���A�C�e���̒��ɒǉ�����A�C�e���Ɠ������̂��Ȃ����`�F�b�N�B
		for (int i = 0; i < UseItemListCounter; i++)
		{
			//�����A�C�e���Ȃ̂ŏ������̍X�V�B
			if (_PlayerItemList[i].ID = item->ID)
			{
				_PlayerItemList[i].HoldNum++;
				return false;
			}
			else
			{
				//�����A�C�e�������������B
				return true;
			}
		}
	}
	//�h��̃`�F�b�N�B
	else if (NULL != armor)
	{
		//�������Ă���h��̒��ɒǉ�����h��Ɠ������̂��Ȃ����`�F�b�N�B
		for (int i = 0; i < UseArmorListCounter; i++)
		{
			//�����h��Ȃ̂ŏ������̍X�V�B
			if (_PlayerArmorList[i].ID = armor->ID)
			{
				_PlayerArmorList[i].HoldNum++;
				return false;
			}
			else
			{
				//�����A�C�e�������������B
				return true;
			}
		}
	}
	//����̃`�F�b�N�B
	else if (NULL != weapon)
	{
		//�������Ă��镐��̒��ɒǉ����镐��Ɠ������̂��Ȃ����`�F�b�N�B
		for (int i = 0; i < UseArmorListCounter; i++)
		{
			//��������Ȃ̂ŏ������̍X�V�B
			if (_PlayerWeapon[i].ID = weapon->ID)
			{
				_PlayerWeapon[i].HoldNum++;
				return false;
			}
			else
			{
				//�����A�C�e�������������B
				return true;
			}
		}
	}
}