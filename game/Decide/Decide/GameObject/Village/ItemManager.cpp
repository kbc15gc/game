#include "stdafx.h"
#include "ItemManager.h"

ItemManager* ItemManager::_Instance = nullptr;

ItemManager::ItemManager()
{
}

void ItemManager::LoadAllItemData()
{
	//�t�@�C���l�[��
	const char* filename[] = { "ItemData","WeaponData","ArmorData", };
	FOR(i, ARRAY_SIZE(filename))
	{
		//�t�@�C���p�X
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/ItemData/%s.csv", filename[i]);
		//�t�@�C������A�C�e�����ǂݍ���
		Support::LoadCSVData<Item::ItemInfo>(filepath, Item::Itemdata, ARRAY_SIZE(Item::Itemdata), _ItemList[i]);

		switch (i)
		{
		case 0:
			//�t�@�C������A�C�e�����ǂݍ��݁B
			Support::LoadCSVData<Item::ItemInfo>(filepath, Item::Itemdata, ARRAY_SIZE(Item::Itemdata), _ItemListVec);
			break;
		case 1:
			//�t�@�C�����畐����ǂݍ��݁B
			Support::LoadCSVData<Weapon::WeaponInfo>(filepath, Weapon::Weapondata, ARRAY_SIZE(Weapon::Weapondata), _WeaponList);
			break;
		case 2:
			//�t�@�C������h����ǂݍ��݁B
			Support::LoadCSVData<Armor::ArmorInfo>(filepath, Armor::Armordata, ARRAY_SIZE(Armor::Armordata), _ArmorList);
			break;
		}
	}
	AddPlayerItemList(ItemKodeE::Item, &_ItemListVec[0]->ID, _ItemListVec[0]->Name, _ItemListVec[0]->Description, &_ItemListVec[0]->Value,  nullptr, nullptr);
	AddPlayerItemList(ItemKodeE::Weapon, &_WeaponList[0]->ID, _ItemListVec[0]->Name, _ItemListVec[0]->Description, &_ItemListVec[0]->Value, &_WeaponList[0]->ATK, nullptr);
	AddPlayerItemList(ItemKodeE::Armor, &_ArmorList[0]->ID, _ArmorList[0]->Name, _ArmorList[0]->Description, &_ArmorList[0]->Value, &_ArmorList[0]->ATK, &_ArmorList[0]->DEF);

}

void ItemManager::AddPlayerItemList(ItemKodeE kode, int* id, const char* name, const char* des, int* value,  int* atk, int* def)
{
	//�ǉ�����A�C�e���̎�ނ𔻕ʁB
	switch (kode)
	{
		//�A�C�e����ǉ��B
	case ItemManager::ItemKodeE::Item:

		//�ő吔�ɒB���Ă��Ȃ����`�F�b�N�B
		if (ItemCounter < ListNum)
		{
			//�n���ꂽ�������ɃA�C�f�A�̏���ǉ��B
			_PlayerItemList[ItemCounter].ID = *id;
			strcpy(_PlayerItemList[ItemCounter].Name, name);
			strcpy(_PlayerItemList[ItemCounter].Description, des);
			_PlayerItemList[ItemCounter].Value = *value;

			//�ǉ����ꂽ�̂ŏ��������X�V�B
			ItemCounter++;
			break;
		}
		else
		{
			//char error[256];
			//sprintf(error, "�v���C���[�̃A�C�e�����X�g�ɃA�C�e����ǉ��ł��܂���ł����B");
			//MessageBoxA(0, error, "�A�C�e���̒ǉ����s", MB_ICONWARNING);
			break;
		}
		//�h���ǉ��B
	case ItemManager::ItemKodeE::Armor:

		//�ő吔�ɒB���Ă��Ȃ����`�F�b�N�B
		if (ArmorCounter < ListNum)
		{
			//�n���ꂽ�������ɃA�C�f�A�̏���ǉ��B
			_PlayerArmorList[ArmorCounter].ID = *id;
			strcpy(_PlayerArmorList[ArmorCounter].Name, name);
			strcpy(_PlayerArmorList[ArmorCounter].Description, des);
			_PlayerArmorList[ItemCounter].Value = *value;
			_PlayerArmorList[ArmorCounter].ATK = *atk;
			_PlayerArmorList[ArmorCounter].DEF = *def;

			//�ǉ����ꂽ�̂ŏ��������X�V�B
			ArmorCounter++;
			break;
		}
		else
		{
			//char error[256];
			//sprintf(error, "�v���C���[�̃A�C�e�����X�g�ɃA�C�e����ǉ��ł��܂���ł����B");
			//MessageBoxA(0, error, "�A�C�e���̒ǉ����s", MB_ICONWARNING);
			break;
		}
		//�����ǉ��B
	case ItemManager::ItemKodeE::Weapon:

		//�ő吔�ɒB���Ă��Ȃ����`�F�b�N�B
		if (WeapoCounter < ListNum)
		{
			//�n���ꂽ�������ɃA�C�f�A�̏���ǉ��B
			_PlayerWeaponList[WeapoCounter].ID = *id;
			strcpy(_PlayerWeaponList[WeapoCounter].Name, name);
			strcpy(_PlayerWeaponList[WeapoCounter].Description, des);
			_PlayerWeaponList[WeapoCounter].Value = *value;
			_PlayerWeaponList[WeapoCounter].ATK = *atk;

			//�ǉ����ꂽ�̂ŏ��������X�V�B
			WeapoCounter++;
			break;
		}
		else
		{
			//char error[256];
			//sprintf(error, "�v���C���[�̃A�C�e�����X�g�ɃA�C�e����ǉ��ł��܂���ł����B");
			//MessageBoxA(0, error, "�A�C�e���̒ǉ����s", MB_ICONWARNING);
			break;
		}
	default:
		char error[256];
		sprintf(error, "�w�肵���A�C�e���R�[�h�������ł��B");
		MessageBoxA(0, error, "�A�C�e���R�[�h�̎w������s", MB_ICONWARNING);
		break;
	}
}

Armor::ArmorInfo *ItemManager::GetPlayerAromorInfo()
{
	return &_PlayerArmorList[0];
}

Item::ItemInfo *ItemManager::GetPlayerItemInfo()
{
	return &_PlayerItemList[0];
}

Weapon::WeaponInfo *ItemManager::GetPlayerWeaponInfo()
{
	return &_PlayerWeaponList[0];
}