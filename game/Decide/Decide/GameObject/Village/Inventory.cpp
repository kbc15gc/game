#include "stdafx.h"
#include "Inventory.h"

Inventory* Inventory::_InventoryInstance = nullptr;

Inventory::Inventory()
{
}

void Inventory::ListInitalize()
{
	//�t�@�C���l�[��
	const char* filename[] = { "Item","Armor","Weapon", };
	FOR(i, ARRAY_SIZE(filename)) {

		//�e���X�g�̏������B
		{
			switch (i)
			{
			case (int)InventoryKodeE::ItemList:
				//�v���C���[�̃A�C�e���̃C���x���g���B
				for (int j = 0; j < INVENTORYLISTNUM; j++)
				{
					_PlayerItemList[j].ID = -1;
					strcpy(_PlayerItemList[j].Name, "None");
					strcpy(_PlayerItemList[j].Description, "None");
					_PlayerItemList[j].Value = -1;
					_PlayerItemList[j].Recovery = -1;
					_PlayerItemList[j].AtkBuff = -1;
					_PlayerItemList[j].DefBuff = -1;
					_PlayerItemList[j].HoldNum = 0;
					_PlayerItemList[j].SpeedBuff = -1;
				}
				break;
			case (int)InventoryKodeE::ArmorList:
				//�v���C���[�̖h��̃C���x���g���B
				for (int j = 0; j < INVENTORYLISTNUM; j++)
				{
					_PlayerArmorList[j].ID = -1;
					strcpy(_PlayerArmorList[j].Name, "None");
					strcpy(_PlayerArmorList[j].Description, "None");
					_PlayerArmorList[j].Value = -1;
					_PlayerArmorList[j].ATK = -1;
					_PlayerArmorList[j].DEF = -1;
					_PlayerArmorList[j].HoldNum = 0;
				}
				break;
			case (int)InventoryKodeE::WeaponList:
				//�v���C���[�̕���̃C���x���g���B
				for (int j = 0; j < INVENTORYLISTNUM; j++)
				{
					_PlayerWeapon[j].ID = -1;
					strcpy(_PlayerWeapon[j].Name, "None");
					strcpy(_PlayerWeapon[j].Description, "None");
					_PlayerWeapon[j].Value = -1;
					_PlayerWeapon[j].ATK = -1;
					_PlayerWeapon[j].HoldNum = 0;
				}
				break;
			}
		}
	}
}

//void Inventory::AddPlayerInventoryList(AddItemKodeE kode,Item::ItemInfo *item, Armor::ArmorInfo *armor, Weapon::WeaponInfo *weapon)
//{
//	switch (kode)
//	{
//		//�v���C���[�̃A�C�e���̃C���x���g���ɒǉ��B
//	case Inventory::AddItemKodeE::Item:
//
//		//�������Ă���A�C�e���̒��ɒǉ�����A�C�e���Ɠ������̂��Ȃ����`�F�b�N�B
//		for (int i = 0; i < ItemCounter; i++)
//		{
//			//�����A�C�e���Ȃ̂ŏ������̍X�V�B
//			if (_PlayerItemList[i].ID = item->ID)
//			{
//				_PlayerItemList[i].HoldNum++;
//				break;
//			}	
//		}
//
//		//�ǉ�����A�C�e���̏���ǉ��B
//		{
//			_PlayerItemList[ItemCounter].ID = item->ID;
//			strcpy(_PlayerItemList[ItemCounter].Name, item->Name);
//			strcpy(_PlayerItemList[ItemCounter].Description, item->Description);
//			_PlayerItemList[ItemCounter].Value = item->Value;
//			_PlayerItemList[ItemCounter].Recovery = item->Recovery;
//			_PlayerItemList[ItemCounter].AtkBuff = item->AtkBuff;
//			_PlayerItemList[ItemCounter].DefBuff = item->DefBuff;
//			_PlayerItemList[ItemCounter].SpeedBuff = item->SpeedBuff;
//			_PlayerItemList[ItemCounter].HoldNum++;
//			ItemCounter++;
//		}
//		break;
//
//		//�v���C���[�̖h��̃C���x���g���ɒǉ��B
//	case Inventory::AddItemKodeE::Armor:
//
//		//�������Ă���h��̒��ɒǉ�����h��Ɠ������̂��Ȃ����`�F�b�N�B
//		for (int i = 0; i < ArmorCounter; i++)
//		{
//			//�����h��Ȃ̂ŏ������̍X�V�B
//			if (_PlayerArmorList[i].ID = item->ID)
//			{
//				_PlayerArmorList[i].HoldNum++;
//				break;
//			}
//		}
//
//		//�ǉ�����h��̏���ǉ��B
//		{
//			_PlayerArmorList[ArmorCounter].ID = armor->ID;
//			strcpy(_PlayerArmorList[ArmorCounter].Name, armor->Name);
//			strcpy(_PlayerArmorList[ArmorCounter].Description, armor->Description);
//			_PlayerArmorList[ArmorCounter].Value = armor->Value;
//			_PlayerArmorList[ArmorCounter].ATK = armor->ATK;
//			_PlayerArmorList[ArmorCounter].DEF = armor->DEF;
//		}
//		break;
//
//		//�v���C���[�̕���̃C���x���g���ɒǉ��B
//	case Inventory::AddItemKodeE::Weapon:
//
//		//�������Ă��镐��̒��ɒǉ����镐��Ɠ������̂��Ȃ����`�F�b�N�B
//		for (int i = 0; i < ArmorCounter; i++)
//		{
//			//��������Ȃ̂ŏ������̍X�V�B
//			if (_PlayerWeapon[i].ID = item->ID)
//			{
//				_PlayerWeapon[i].HoldNum++;
//				break;
//			}
//		}
//
//		//�ǉ����镐��̏���ǉ��B
//		{
//			_PlayerWeapon[WeaponCounter].ID = armor->ID;
//			strcpy(_PlayerWeapon[WeaponCounter].Name, armor->Name);
//			strcpy(_PlayerArmorList[WeaponCounter].Description, armor->Description);
//			_PlayerWeapon[WeaponCounter].Value = armor->Value;
//			_PlayerWeapon[WeaponCounter].ATK = armor->ATK;
//		}
//		break;
//	default:
//		char error[256];
//		sprintf(error, "�w�肵���A�C�e���R�[�h�������ł��B");
//		MessageBoxA(0, error, "�A�C�e���R�[�h�̎w������s", MB_ICONWARNING);
//		break;
//	}
//}

void Inventory::AddPlayerInventoryItem(Item::ItemInfo *item)
{
	//�������Ă���A�C�e���̒��ɒǉ�����A�C�e���Ɠ������̂��Ȃ����`�F�b�N�B
	for (int i = 0; i < ItemCounter; i++)
	{
		//�����A�C�e���Ȃ̂ŏ������̍X�V�B
		if (_PlayerItemList[i].ID = item->ID)
		{
			_PlayerItemList[i].HoldNum++;
			break;
		}
	}

	//�ǉ�����A�C�e���̏���ǉ��B
	{
		_PlayerItemList[ItemCounter].ID = item->ID;
		strcpy(_PlayerItemList[ItemCounter].Name, item->Name);
		strcpy(_PlayerItemList[ItemCounter].Description, item->Description);
		_PlayerItemList[ItemCounter].Value = item->Value;
		_PlayerItemList[ItemCounter].Recovery = item->Recovery;
		_PlayerItemList[ItemCounter].AtkBuff = item->AtkBuff;
		_PlayerItemList[ItemCounter].DefBuff = item->DefBuff;
		_PlayerItemList[ItemCounter].SpeedBuff = item->SpeedBuff;
		_PlayerItemList[ItemCounter].HoldNum++;
		ItemCounter++;
	}
}

void Inventory::AddPlayerInventoryIArmor(Armor::ArmorInfo *armor)
{
	//�������Ă���h��̒��ɒǉ�����h��Ɠ������̂��Ȃ����`�F�b�N�B
	for (int i = 0; i < ArmorCounter; i++)
	{
		//�����h��Ȃ̂ŏ������̍X�V�B
		if (_PlayerArmorList[i].ID = armor->ID)
		{
			_PlayerArmorList[i].HoldNum++;
			break;
		}
	}

	//�ǉ�����h��̏���ǉ��B
	{
		_PlayerArmorList[ArmorCounter].ID = armor->ID;
		strcpy(_PlayerArmorList[ArmorCounter].Name, armor->Name);
		strcpy(_PlayerArmorList[ArmorCounter].Description, armor->Description);
		_PlayerArmorList[ArmorCounter].Value = armor->Value;
		_PlayerArmorList[ArmorCounter].ATK = armor->ATK;
		_PlayerArmorList[ArmorCounter].DEF = armor->DEF;
	}
}

void Inventory::AddPlayerInventoryWeapon(Weapon::WeaponInfo *weapon)
{
	//�������Ă��镐��̒��ɒǉ����镐��Ɠ������̂��Ȃ����`�F�b�N�B
	for (int i = 0; i < ArmorCounter; i++)
	{
		//��������Ȃ̂ŏ������̍X�V�B
		if (_PlayerWeapon[i].ID = weapon->ID)
		{
			_PlayerWeapon[i].HoldNum++;
			break;
		}
	}

	//�ǉ����镐��̏���ǉ��B
	{
		_PlayerWeapon[WeaponCounter].ID = weapon->ID;
		strcpy(_PlayerWeapon[WeaponCounter].Name, weapon->Name);
		strcpy(_PlayerArmorList[WeaponCounter].Description, weapon->Description);
		_PlayerWeapon[WeaponCounter].Value = weapon->Value;
		_PlayerWeapon[WeaponCounter].ATK = weapon->ATK;
	}
}