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
		Support::LoadCSVData<ItemBase::Item::ItemInfo>(filepath, ItemBase::Item::Itemdata, ARRAY_SIZE(ItemBase::Item::Itemdata), _ItemList[i]);

		switch (i)
		{
		case (int)ItemKodeE::Item:
			//�t�@�C������A�C�e�����ǂݍ��݁B
			Support::LoadCSVData<ItemBase::Item::ItemInfo>(filepath, ItemBase::Item::Itemdata, ARRAY_SIZE(ItemBase::Item::Itemdata), _ItemListVec);
			break;
		case (int)ItemKodeE::Armor:
			//�t�@�C������h����ǂݍ��݁B
			Support::LoadCSVData<ItemBase::Armor::ArmorInfo>(filepath, ItemBase::Armor::Armordata, ARRAY_SIZE(ItemBase::Armor::Armordata), _ArmorList);
			break;	
		case (int)ItemKodeE::Weapon:	
			//�t�@�C�����畐����ǂݍ��݁B
			Support::LoadCSVData<ItemBase::Weapon::WeaponInfo>(filepath, ItemBase::Weapon::Weapondata, ARRAY_SIZE(ItemBase::Weapon::Weapondata), _WeaponList);
			break;
		}
	}
	/*AddPlayerItemList(ItemKodeE::Item, &_ItemListVec[0]->ID, _ItemListVec[0]->Name, _ItemListVec[0]->Description, &_ItemListVec[0]->Value,  nullptr, nullptr);
	AddPlayerItemList(ItemKodeE::Weapon, &_WeaponList[0]->ID, _ItemListVec[0]->Name, _ItemListVec[0]->Description, &_ItemListVec[0]->Value, &_WeaponList[0]->ATK, nullptr);
	AddPlayerItemList(ItemKodeE::Armor, &_ArmorList[0]->ID, _ArmorList[0]->Name, _ArmorList[0]->Description, &_ArmorList[0]->Value, &_ArmorList[0]->ATK, &_ArmorList[0]->DEF);*/
}

//void ItemManager::AddPlayerItemList(ItemKodeE kode, int* id, const char* name, const char* des, int* value,  int* atk, int* def)
//{
//	//�ǉ�����A�C�e���̎�ނ𔻕ʁB
//	switch (kode)
//	{
//		//�A�C�e����ǉ��B
//	case ItemManager::ItemKodeE::Item:
//
//		//�ő吔�ɒB���Ă��Ȃ����`�F�b�N�B
//		if (ItemCounter < ListNum)
//		{
//			//�n���ꂽ�������ɃA�C�f�A�̏���ǉ��B
//			_PlayerItemList[ItemCounter].ID = *id;
//			strcpy(_PlayerItemList[ItemCounter].Name, name);
//			strcpy(_PlayerItemList[ItemCounter].Description, des);
//			_PlayerItemList[ItemCounter].Value = *value;
//
//			//�ǉ����ꂽ�̂ŏ��������X�V�B
//			ItemCounter++;
//			break;
//		}
//		else
//		{
//			//char error[256];
//			//sprintf(error, "�v���C���[�̃A�C�e�����X�g�ɃA�C�e����ǉ��ł��܂���ł����B");
//			//MessageBoxA(0, error, "�A�C�e���̒ǉ����s", MB_ICONWARNING);
//			break;
//		}
//		//�h���ǉ��B
//	case ItemManager::ItemKodeE::Armor:
//
//		//�ő吔�ɒB���Ă��Ȃ����`�F�b�N�B
//		if (ArmorCounter < ListNum)
//		{
//			//�n���ꂽ�������ɃA�C�f�A�̏���ǉ��B
//			_PlayerArmorList[ArmorCounter].ID = *id;
//			strcpy(_PlayerArmorList[ArmorCounter].Name, name);
//			strcpy(_PlayerArmorList[ArmorCounter].Description, des);
//			_PlayerArmorList[ItemCounter].Value = *value;
//			_PlayerArmorList[ArmorCounter].ATK = *atk;
//			_PlayerArmorList[ArmorCounter].DEF = *def;
//
//			//�ǉ����ꂽ�̂ŏ��������X�V�B
//			ArmorCounter++;
//			break;
//		}
//		else
//		{
//			//char error[256];
//			//sprintf(error, "�v���C���[�̃A�C�e�����X�g�ɃA�C�e����ǉ��ł��܂���ł����B");
//			//MessageBoxA(0, error, "�A�C�e���̒ǉ����s", MB_ICONWARNING);
//			break;
//		}
//		//�����ǉ��B
//	case ItemManager::ItemKodeE::Weapon:
//
//		//�ő吔�ɒB���Ă��Ȃ����`�F�b�N�B
//		if (WeapoCounter < ListNum)
//		{
//			//�n���ꂽ�������ɃA�C�f�A�̏���ǉ��B
//			_PlayerWeaponList[WeapoCounter].ID = *id;
//			strcpy(_PlayerWeaponList[WeapoCounter].Name, name);
//			strcpy(_PlayerWeaponList[WeapoCounter].Description, des);
//			_PlayerWeaponList[WeapoCounter].Value = *value;
//			_PlayerWeaponList[WeapoCounter].ATK = *atk;
//
//			//�ǉ����ꂽ�̂ŏ��������X�V�B
//			WeapoCounter++;
//			break;
//		}
//		else
//		{
//			//char error[256];
//			//sprintf(error, "�v���C���[�̃A�C�e�����X�g�ɃA�C�e����ǉ��ł��܂���ł����B");
//			//MessageBoxA(0, error, "�A�C�e���̒ǉ����s", MB_ICONWARNING);
//			break;
//		}
//	default:
//		char error[256];
//		sprintf(error, "�w�肵���A�C�e���R�[�h�������ł��B");
//		MessageBoxA(0, error, "�A�C�e���R�[�h�̎w������s", MB_ICONWARNING);
//		break;
//	}
//}