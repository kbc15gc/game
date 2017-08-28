#include "stdafx.h"
#include "Inventory.h"

Inventory* Inventory::_InventoryInstance = nullptr;

Inventory::Inventory()
{
}

//各インベントリの初期化。
void Inventory::ListInitalize()
{
	//ファイルネーム
	const char* filename[] = { "Item","Armor","Weapon", };
	FOR(i, ARRAY_SIZE(filename)) {
		switch (i)
		{
			//プレイヤーのアイテムのインベントリ。
		case (int)ItemManager::ItemKodeE::Item:

			for (int j = 0; j < INVENTORYLISTNUM; j++)
			{
				_PlayerItemListInitialize(j);
			}
			break;
			//プレイヤーの防具のインベントリ。
		case (int)ItemManager::ItemKodeE::Armor:

			for (int j = 0; j < INVENTORYLISTNUM; j++)
			{
				_PlayerArmorListInitialize(j);
			}
			break;
			//プレイヤーの武器のインベントリ。
		case (int)ItemManager::ItemKodeE::Weapon:

			for (int j = 0; j < INVENTORYLISTNUM; j++)
			{
				_PlayerWeaponListInitialize(j);
			}
			break;
		}
	}
}

//プレイヤーのアイテムのインベントリに追加。
void Inventory::AddPlayerInventoryItem(Item::ItemInfo *item)
{
	////所持しているアイテムの中に追加するアイテムと同じものがないかチェック。
	//for (int i = 0; i < UseItemListCounter; i++)
	//{
	//	//同じアイテムなので所持数の更新。
	//	if (_PlayerItemList[i].ID = item->ID)
	//	{
	//		_PlayerItemList[i].HoldNum++;
	//		break;
	//	}
	//}

	//if (_AddCheck(item, NULL, NULL) != false) {

		//追加するアイテムの情報を追加。
		_AddItem(item);

	//}

	
}

//プレイヤーの防具のインベントリに追加。
void Inventory::AddPlayerInventoryIArmor(Item::ArmorInfo *armor)
{
	////所持している防具の中に追加する防具と同じものがないかチェック。
	//for (int i = 0; i < UseArmorListCounter; i++)
	//{
	//	//同じ防具なので所持数の更新。
	//	if (_PlayerArmorList[i].ID = armor->ID)
	//	{
	//		_PlayerArmorList[i].HoldNum++;
	//		break;
	//	}
	//}

	//if (_AddCheck(NULL, armor, NULL) != false) {

		//追加する防具の情報を追加。
		_AddArmor(armor);

	//}
}

//プレイヤーの武器のインベントリに追加。
void Inventory::AddPlayerInventoryWeapon(Item::WeaponInfo *weapon)
{
	////所持している武器の中に追加する武器と同じものがないかチェック。
	//for (int i = 0; i < UseArmorListCounter; i++)
	//{
	//	//同じ武器なので所持数の更新。
	//	if (_PlayerWeapon[i].ID = weapon->ID)
	//	{
	//		_PlayerWeapon[i].HoldNum++;
	//		break;
	//	}
	//}

	//if (_AddCheck(NULL, NULL,weapon) != false) {

		//追加する武器の情報を追加。
		_AddWeapon(weapon);
	//}
	
}

void Inventory::AddInventory(ItemManager::ItemKodeE kode ,Item::BaseInfo * item)
{
	switch (kode)
	{
	case ItemManager::ItemKodeE::Item:
		_AddItem((Item::ItemInfo*)item);
		break;

	case ItemManager::ItemKodeE::Armor:
		_AddArmor((Item::ArmorInfo*)item);
		break;

	case ItemManager::ItemKodeE::Weapon:
		_AddWeapon((Item::WeaponInfo*)item);
		break;
	default:
		break;
	}
}

//プレイヤーのアイテムのインベントリを初期化。
void Inventory::_PlayerItemListInitialize(int i) {
	_PlayerItemList[i].ID = -1;
	strcpy(_PlayerItemList[i].Name, "None");
	strcpy(_PlayerItemList[i].Description, "None");
	_PlayerItemList[i].Value = -1;
	_PlayerItemList[i].Recovery = -1;
	_PlayerItemList[i].AtkBuff = -1;
	_PlayerItemList[i].DefBuff = -1;
	_PlayerItemList[i].HoldNum = 0;
	_PlayerItemList[i].SpeedBuff = -1;
}

//プレイヤーの防具のインベントリを初期化。
void Inventory::_PlayerArmorListInitialize(int i)
{
	_PlayerArmorList[i].ID = -1;
	strcpy(_PlayerArmorList[i].Name, "None");
	strcpy(_PlayerArmorList[i].Description, "None");
	_PlayerArmorList[i].Value = -1;
	_PlayerArmorList[i].ATK = -1;
	_PlayerArmorList[i].DEF = -1;
	_PlayerArmorList[i].HoldNum = 0;
}

//プレイヤーの武器のインベントリを初期化。
void Inventory::_PlayerWeaponListInitialize(int i)
{
	_PlayerWeapon[i].ID = -1;
	strcpy(_PlayerWeapon[i].Name, "None");
	strcpy(_PlayerWeapon[i].Description, "None");
	_PlayerWeapon[i].Value = -1;
	_PlayerWeapon[i].ATK = -1;
	_PlayerWeapon[i].HoldNum = 0;
}

//アイテムをインベントリに追加。
void Inventory::_AddItem(Item::ItemInfo *item)
{
	_PlayerItemList[UseItemListCounter].TypeID = item->TypeID;
	_PlayerItemList[UseItemListCounter].ID = item->ID;
	strcpy(_PlayerItemList[UseItemListCounter].Name, item->Name);
	strcpy(_PlayerItemList[UseItemListCounter].Description, item->Description);
	_PlayerItemList[UseItemListCounter].Value = item->Value;
	_PlayerItemList[UseItemListCounter].Recovery = item->Recovery;
	_PlayerItemList[UseItemListCounter].AtkBuff = item->AtkBuff;
	_PlayerItemList[UseItemListCounter].DefBuff = item->DefBuff;
	_PlayerItemList[UseItemListCounter].SpeedBuff = item->SpeedBuff;
	_PlayerItemList[UseItemListCounter].HoldNum++;
	UseItemListCounter++;

}

//防具をインベントリに追加。
void Inventory::_AddArmor(Item::ArmorInfo *armor)
{
	_PlayerArmorList[UseArmorListCounter].TypeID = armor->TypeID;
	_PlayerArmorList[UseArmorListCounter].ID = armor->ID;
	strcpy(_PlayerArmorList[UseArmorListCounter].Name, armor->Name);
	strcpy(_PlayerArmorList[UseArmorListCounter].Description, armor->Description);
	_PlayerArmorList[UseArmorListCounter].Value = armor->Value;
	_PlayerArmorList[UseArmorListCounter].ATK = armor->ATK;
	_PlayerArmorList[UseArmorListCounter].DEF = armor->DEF;
	UseArmorListCounter++;
}

//武器をインベントリに追加。
void Inventory::_AddWeapon(Item::WeaponInfo *weapon)
{
	_PlayerWeapon[UseWeaponListCounter].TypeID = weapon->TypeID;
	_PlayerWeapon[UseWeaponListCounter].ID = weapon->ID;
	strcpy(_PlayerWeapon[UseWeaponListCounter].Name, weapon->Name);
	strcpy(_PlayerArmorList[UseWeaponListCounter].Description, weapon->Description);
	_PlayerWeapon[UseWeaponListCounter].Value = weapon->Value;
	_PlayerWeapon[UseWeaponListCounter].ATK = weapon->ATK;
	UseWeaponListCounter++;
}

//追加するアイテムがすでに追加されているかチェックし無ければtrueを返す
bool Inventory::_AddCheck(Item::ItemInfo *item , Item::ArmorInfo *armor, Item::WeaponInfo *weapon)
{
	//アイテムのチェック。
	if (NULL != item) 
	{
		//所持しているアイテムの中に追加するアイテムと同じものがないかチェック。
		for (int i = 0; i < UseItemListCounter; i++)
		{
			//同じアイテムなので所持数の更新。
			if (_PlayerItemList[i].ID = item->ID)
			{
				_PlayerItemList[i].HoldNum++;
				return false;
			}
			else
			{
				//同じアイテムが無かった。
				return true;
			}
		}
	}
	//防具のチェック。
	else if (NULL != armor)
	{
		//所持している防具の中に追加する防具と同じものがないかチェック。
		for (int i = 0; i < UseArmorListCounter; i++)
		{
			//同じ防具なので所持数の更新。
			if (_PlayerArmorList[i].ID = armor->ID)
			{
				_PlayerArmorList[i].HoldNum++;
				return false;
			}
			else
			{
				//同じアイテムが無かった。
				return true;
			}
		}
	}
	//武器のチェック。
	else if (NULL != weapon)
	{
		//所持している武器の中に追加する武器と同じものがないかチェック。
		for (int i = 0; i < UseArmorListCounter; i++)
		{
			//同じ武器なので所持数の更新。
			if (_PlayerWeapon[i].ID = weapon->ID)
			{
				_PlayerWeapon[i].HoldNum++;
				return false;
			}
			else
			{
				//同じアイテムが無かった。
				return true;
			}
		}
	}
}