#include "stdafx.h"
#include "ItemManager.h"

ItemManager* ItemManager::_Instance = nullptr;

ItemManager::ItemManager()
{
}

void ItemManager::LoadItemData()
{
	//�t�@�C���l�[��
	const char* filename[] = { "","WeaponData","ArmorData", };
	FOR(i, ARRAY_SIZE(filename))
	{
		//�t�@�C���p�X
		char filepath[256] = "";
		sprintf(filepath, "Asset/Data/ItemData/%s.csv", filename[i]);
		//�t�@�C������A�C�e�����ǂݍ���
		Support::LoadCSVData<Item::ItemInfo>(filepath, Item::Itemdata, ARRAY_SIZE(Item::Itemdata), _ItemList[i]);
	}
}