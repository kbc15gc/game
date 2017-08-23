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
		case 0:
			//ファイルからアイテム情報読み込み。
			Support::LoadCSVData<Item::ItemInfo>(filepath, Item::Itemdata, ARRAY_SIZE(Item::Itemdata), _ItemListVec);
			break;
		case 1:
			//ファイルから武器情報読み込み。
			Support::LoadCSVData<Weapon::WeaponInfo>(filepath, Weapon::Weapondata, ARRAY_SIZE(Weapon::Weapondata), _WeaponList);
			break;
		case 2:
			//ファイルから防具情報読み込み。
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
	//追加するアイテムの種類を判別。
	switch (kode)
	{
		//アイテムを追加。
	case ItemManager::ItemKodeE::Item:

		//最大数に達していないかチェック。
		if (ItemCounter < ListNum)
		{
			//渡された情報を元にアイデアの情報を追加。
			_PlayerItemList[ItemCounter].ID = *id;
			strcpy(_PlayerItemList[ItemCounter].Name, name);
			strcpy(_PlayerItemList[ItemCounter].Description, des);
			_PlayerItemList[ItemCounter].Value = *value;

			//追加されたので所持数を更新。
			ItemCounter++;
			break;
		}
		else
		{
			//char error[256];
			//sprintf(error, "プレイヤーのアイテムリストにアイテムを追加できませんでした。");
			//MessageBoxA(0, error, "アイテムの追加失敗", MB_ICONWARNING);
			break;
		}
		//防具を追加。
	case ItemManager::ItemKodeE::Armor:

		//最大数に達していないかチェック。
		if (ArmorCounter < ListNum)
		{
			//渡された情報を元にアイデアの情報を追加。
			_PlayerArmorList[ArmorCounter].ID = *id;
			strcpy(_PlayerArmorList[ArmorCounter].Name, name);
			strcpy(_PlayerArmorList[ArmorCounter].Description, des);
			_PlayerArmorList[ItemCounter].Value = *value;
			_PlayerArmorList[ArmorCounter].ATK = *atk;
			_PlayerArmorList[ArmorCounter].DEF = *def;

			//追加されたので所持数を更新。
			ArmorCounter++;
			break;
		}
		else
		{
			//char error[256];
			//sprintf(error, "プレイヤーのアイテムリストにアイテムを追加できませんでした。");
			//MessageBoxA(0, error, "アイテムの追加失敗", MB_ICONWARNING);
			break;
		}
		//武器を追加。
	case ItemManager::ItemKodeE::Weapon:

		//最大数に達していないかチェック。
		if (WeapoCounter < ListNum)
		{
			//渡された情報を元にアイデアの情報を追加。
			_PlayerWeaponList[WeapoCounter].ID = *id;
			strcpy(_PlayerWeaponList[WeapoCounter].Name, name);
			strcpy(_PlayerWeaponList[WeapoCounter].Description, des);
			_PlayerWeaponList[WeapoCounter].Value = *value;
			_PlayerWeaponList[WeapoCounter].ATK = *atk;

			//追加されたので所持数を更新。
			WeapoCounter++;
			break;
		}
		else
		{
			//char error[256];
			//sprintf(error, "プレイヤーのアイテムリストにアイテムを追加できませんでした。");
			//MessageBoxA(0, error, "アイテムの追加失敗", MB_ICONWARNING);
			break;
		}
	default:
		char error[256];
		sprintf(error, "指定したアイテムコードが無効です。");
		MessageBoxA(0, error, "アイテムコードの指定を失敗", MB_ICONWARNING);
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