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

	FOR(i, Item::ItemCodeE::Max)
	{
		//�t�@�C���p�X
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/ItemData/%s.csv", filename[i]);
		switch (i)
		{
		case (int)Item::ItemCodeE::Item:
			//�t�@�C������A�C�e�����ǂݍ��݁B
			Support::LoadCSVData<Item::ItemInfo,Item::BaseInfo>(filepath, Item::Itemdata, ARRAY_SIZE(Item::Itemdata), _InfoList[i]);
			break;
		case (int)Item::ItemCodeE::Armor:
			//�t�@�C������h����ǂݍ��݁B
			Support::LoadCSVData<Item::ArmorInfo, Item::BaseInfo>(filepath, Item::Armordata, ARRAY_SIZE(Item::Armordata), _InfoList[i]);
			break;
		case (int)Item::ItemCodeE::Weapon:
			//�t�@�C�����畐����ǂݍ��݁B
			Support::LoadCSVData<Item::WeaponInfo, Item::BaseInfo>(filepath, Item::Weapondata, ARRAY_SIZE(Item::Weapondata), _InfoList[i]);
			break;
		}
	}

	for (int idx = 0; idx < static_cast<int>(Item::ItemCodeE::Max); idx++) {
		_maxID[idx] = 0;	// �������B
		for (auto& info : _InfoList[idx]) {
			if (info->ID > _maxID[idx]) {
				// �ő�ID�X�V�B

				_maxID[idx] = info->ID;
			}
		}
	}
}