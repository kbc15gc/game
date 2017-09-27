#include "stdafx.h"
#include "ItemManager.h"
#include "GameObject\Inventory\Inventory.h"

ItemManager* ItemManager::_Instance = nullptr;

ItemManager::ItemManager()
{
}

void ItemManager::LoadAllItemData()
{
	//ファイルネーム
	const char* filename[] = { "ItemData","ArmorData","WeaponData" };

	FOR(i, Item::ItemCodeE::Max)
	{
		//ファイルパス
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/ItemData/%s.csv", filename[i]);
		switch (i)
		{
		case (int)Item::ItemCodeE::Item:
			//ファイルからアイテム情報読み込み。
			Support::LoadCSVData<Item::ItemInfo,Item::BaseInfo>(filepath, Item::Itemdata, ARRAY_SIZE(Item::Itemdata), _InfoList[i]);
			break;
		case (int)Item::ItemCodeE::Armor:
			//ファイルから防具情報読み込み。
			Support::LoadCSVData<Item::ArmorInfo, Item::BaseInfo>(filepath, Item::Armordata, ARRAY_SIZE(Item::Armordata), _InfoList[i]);
			break;
		case (int)Item::ItemCodeE::Weapon:
			//ファイルから武器情報読み込み。
			Support::LoadCSVData<Item::WeaponInfo, Item::BaseInfo>(filepath, Item::Weapondata, ARRAY_SIZE(Item::Weapondata), _InfoList[i]);
			break;
		}
	}

	for (int idx = 0; idx < static_cast<int>(Item::ItemCodeE::Max); idx++) {
		_maxID[idx] = 0;	// 初期化。
		for (auto& info : _InfoList[idx]) {
			if (info->ID > _maxID[idx]) {
				// 最大ID更新。

				_maxID[idx] = info->ID;
			}
		}
	}
}