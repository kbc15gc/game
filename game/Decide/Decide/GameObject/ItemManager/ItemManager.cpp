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
	FOR(i, ARRAY_SIZE(filename))
	{
		//ファイルパス
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/ItemData/%s.csv", filename[i]);
		switch (i)
		{
		case (int)Item::ItemCodeE::Item:
			//ファイルからアイテム情報読み込み。
			Support::LoadCSVData<Item::ItemInfo>(filepath, Item::Itemdata, ARRAY_SIZE(Item::Itemdata), _ItemListVec);
			break;
		case (int)Item::ItemCodeE::Armor:
			//ファイルから防具情報読み込み。
			Support::LoadCSVData<Item::ArmorInfo>(filepath, Item::Armordata, ARRAY_SIZE(Item::Armordata), _ArmorList);
			break;
		case (int)Item::ItemCodeE::Weapon:
			//ファイルから武器情報読み込み。
			Support::LoadCSVData<Item::WeaponInfo>(filepath, Item::Weapondata, ARRAY_SIZE(Item::Weapondata), _WeaponList);
			break;
		}
	}
}