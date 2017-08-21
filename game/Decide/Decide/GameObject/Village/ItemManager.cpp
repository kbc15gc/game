#include "stdafx.h"
#include "ItemManager.h"

ItemManager* ItemManager::_Instance = nullptr;

ItemManager::ItemManager()
{
}

void ItemManager::LoadAllData()
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
	}
}

void ItemManager::LoadItemData()
{

}

void ItemManager::LoadArmorData()
{

}

void ItemManager::LoadWeaponData()
{

}