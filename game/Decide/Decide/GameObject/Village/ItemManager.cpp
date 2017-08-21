#include "stdafx.h"
#include "ItemManager.h"

ItemManager* ItemManager::_Instance = nullptr;

ItemManager::ItemManager()
{
}

void ItemManager::LoadAllItemData()
{
	//ファイルネーム
	const char* filename[] = { "ItemData","WeaponData","ArmorData", };
	FOR(i, ARRAY_SIZE(filename))
	{
		//ファイルパス
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/ItemData/%s.csv", filename[i]);
		//ファイルからアイテム情報読み込み
		Support::LoadCSVData<Item::ItemInfo>(filepath, Item::Itemdata, ARRAY_SIZE(Item::Itemdata), _ItemList[i]);

		switch (i)
		{
		case 1:
			//ファイルからアイテム情報読み込み。
			Support::LoadCSVData<Item::ItemInfo>(filepath, Item::Itemdata, ARRAY_SIZE(Item::Itemdata), _ItemListVec);
			break;
		case 2:
			//ファイルから武器情報読み込み。
			Support::LoadCSVData<Weapon::WeaponInfo>(filepath, Weapon::Weapondata, ARRAY_SIZE(Weapon::Weapondata), _WeaponList);
			break;
		case 3:
			//ファイルから防具情報読み込み。
			Support::LoadCSVData<Armor::ArmorInfo>(filepath, Armor::Armordata, ARRAY_SIZE(Armor::Armordata), _ArmorList);
			break;
		}
	}
}