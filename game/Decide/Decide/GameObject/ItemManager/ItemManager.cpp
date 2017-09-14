#include "stdafx.h"
#include "ItemManager.h"
#include "GameObject\Inventory\Inventory.h"

ItemManager* ItemManager::_Instance = nullptr;

ItemManager::ItemManager()
{
}

void ItemManager::LoadAllItemData()
{
	//�t�@�C���l�[��
	const char* filename[] = { "ItemData","ArmorData","WeaponData" };
	FOR(i, ARRAY_SIZE(filename))
	{
		//�t�@�C���p�X
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/ItemData/%s.csv", filename[i]);
		switch (i)
		{
		case (int)Item::ItemCodeE::Item:
			//�t�@�C������A�C�e�����ǂݍ��݁B
			Support::LoadCSVData<Item::ItemInfo>(filepath, Item::Itemdata, ARRAY_SIZE(Item::Itemdata), _ItemListVec);
			break;
		case (int)Item::ItemCodeE::Armor:
			//�t�@�C������h����ǂݍ��݁B
			Support::LoadCSVData<Item::ArmorInfo>(filepath, Item::Armordata, ARRAY_SIZE(Item::Armordata), _ArmorList);
			break;
		case (int)Item::ItemCodeE::Weapon:
			//�t�@�C�����畐����ǂݍ��݁B
			Support::LoadCSVData<Item::WeaponInfo>(filepath, Item::Weapondata, ARRAY_SIZE(Item::Weapondata), _WeaponList);
			break;
		}
	}
}