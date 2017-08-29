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
//プレイヤーのアイテムのインベントリを初期化。
void Inventory::_PlayerItemListInitialize(int i) {
	_PlayerItemList[i].TypeID = -1;
	_PlayerItemList[i].ID = -1;
	strcpy(_PlayerItemList[i].Name, "None");
	strcpy(_PlayerItemList[i].Description, "None");
	_PlayerItemList[i].Value = -1;
	_PlayerItemList[i].Recovery = -1;
	_PlayerItemList[i].AtkBuff = -1;
	_PlayerItemList[i].DefBuff = -1;
	_PlayerItemList[i].SpeedBuff = -1;
	_PlayerItemList[i].HoldNum = 0;
}

//プレイヤーの防具のインベントリを初期化。
void Inventory::_PlayerArmorListInitialize(int i)
{
	_PlayerArmorList[i].TypeID = -1;
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
	_PlayerWeaponList[i].TypeID = -1;
	_PlayerWeaponList[i].ID = -1;
	strcpy(_PlayerWeaponList[i].Name, "None");
	strcpy(_PlayerWeaponList[i].Description, "None");
	_PlayerWeaponList[i].Value = -1;
	_PlayerWeaponList[i].ATK = -1;
	_PlayerWeaponList[i].HoldNum = 0;
}

//インベントリにアイテムを追加する。
void Inventory::AddInventory(ItemManager::ItemKodeE kode, Item::BaseInfo * item)
{
	//NULLチェック。
	if (NULL != item)
	{
		switch (kode)
		{
			//アイテムの追加。
		case ItemManager::ItemKodeE::Item:
			//追加するアイテムか所持数を増やすアイテムかチェック。
			_ItemAddCheckAndPos((Item::ItemInfo*)item);	
			break;

			//防具の追加。
		case ItemManager::ItemKodeE::Armor:
			//追加する防具か所持数を増やすアイテムかチェック。
			_ArmorAddCheckAndPos((Item::ArmorInfo*)item);
			break;

			//武器の追加。
		case ItemManager::ItemKodeE::Weapon:
			//追加する武器か所持数を増やすアイテムかチェック。
			_WeaponAddCheckAndPos((Item::WeaponInfo*)item);
			break;

			//例外処理。
		default:
			char error[256];
			sprintf(error, "指定したアイテムコードコードが無効です。");
			MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
			break;
		}
	}
	else
	{
		char error[256];
		sprintf(error, "追加するアイテムがNULLでした。");
		MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
	}
}

//追加するアイテムをアイテムのインベントリから探しあれば所持数だけを増やし、なければ空いている場所に追加し、どちらでもなければエラーを出す。
void Inventory::_ItemAddCheckAndPos(Item::ItemInfo *item)
{
	//アイテムのインベントリの空きを検索。
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//空きチェック。
		if (_PlayerItemList[i].TypeID == -1) {

			//追加処理。
			_AddItem(i, item);
			return;

		}
		//同じアイテムを検索。
		else if(_PlayerItemList[i].ID == item->ID)
		{
			//所持数を増加。
			_PlayerItemList[i].HoldNum++;
			return;
		}
	}

	char error[256];
	sprintf(error, "アイテムのインベントリが一杯で追加できませんでした。");
	MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
}

void Inventory::_ArmorAddCheckAndPos(Item::ArmorInfo *armor)
{
	//アイテムのインベントリの空きを検索。
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//空きチェック。
		if (_PlayerArmorList[i].TypeID == -1) {

			//追加処理。
			_AddArmor(i, armor);
			return;

		}
		//同じアイテムを検索。
		else if (_PlayerArmorList[i].ID == armor->ID)
		{
			//所持数を増加。
			_PlayerArmorList[i].HoldNum++;
			return;
		}
	}

	char error[256];
	sprintf(error, "防具のインベントリが一杯で追加できませんでした。");
	MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
}

void Inventory::_WeaponAddCheckAndPos(Item::WeaponInfo* weapon)
{
	//アイテムのインベントリの空きを検索。
	for (int i = 0; i < INVENTORYLISTNUM; i++)
	{
		//空きチェック。
		if (_PlayerArmorList[i].TypeID == -1) {

			//追加処理。
			_AddWeapon(i, weapon);
			return;

		}
		//同じアイテムを検索。
		else if (_PlayerArmorList[i].ID == weapon->ID)
		{
			//所持数を増加。
			_PlayerArmorList[i].HoldNum++;
			return;
		}
	}
	char error[256];
	sprintf(error, "アイテムのインベントリが一杯で追加できませんでした。");
	MessageBoxA(0, error, "インベントリに追加失敗", MB_ICONWARNING);
}

//アイテムをインベントリに追加。
void Inventory::_AddItem(int AddPos, Item::ItemInfo *item)
{
	_PlayerItemList[AddPos].TypeID = item->TypeID;
	_PlayerItemList[AddPos].ID = item->ID;
	strcpy(_PlayerItemList[AddPos].Name, item->Name);
	strcpy(_PlayerItemList[AddPos].Description, item->Description);
	_PlayerItemList[AddPos].Value = item->Value;
	_PlayerItemList[AddPos].Recovery = item->Recovery;
	_PlayerItemList[AddPos].AtkBuff = item->AtkBuff;
	_PlayerItemList[AddPos].DefBuff = item->DefBuff;
	_PlayerItemList[AddPos].SpeedBuff = item->SpeedBuff;
	_PlayerItemList[AddPos].HoldNum++;
}

//防具をインベントリに追加。
void Inventory::_AddArmor(int AddPos, Item::ArmorInfo *armor)
{
	_PlayerArmorList[AddPos].TypeID = armor->TypeID;
	_PlayerArmorList[AddPos].ID = armor->ID;
	strcpy(_PlayerArmorList[AddPos].Name, armor->Name);
	strcpy(_PlayerArmorList[AddPos].Description, armor->Description);
	_PlayerArmorList[AddPos].Value = armor->Value;
	_PlayerArmorList[AddPos].ATK = armor->ATK;
	_PlayerArmorList[AddPos].DEF = armor->DEF;
}

//武器をインベントリに追加。
void Inventory::_AddWeapon(int AddPos, Item::WeaponInfo *weapon)
{
	_PlayerWeaponList[AddPos].TypeID = weapon->TypeID;
	_PlayerWeaponList[AddPos].ID = weapon->ID;
	strcpy(_PlayerWeaponList[AddPos].Name, weapon->Name);
	strcpy(_PlayerArmorList[AddPos].Description, weapon->Description);
	_PlayerWeaponList[AddPos].Value = weapon->Value;
	_PlayerWeaponList[AddPos].ATK = weapon->ATK;
	UseWeaponListCounter++;
}